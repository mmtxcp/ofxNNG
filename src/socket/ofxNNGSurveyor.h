#pragma once

#include <stddef.h>
#include "nng.h"
#include "protocol/survey0/survey.h"
#include "supplemental/util/platform.h"
//#include "ofx/utils/ofLog.h"
#include "util/ASyncWork.h"
#include "message/ofxNNGMessage.h"
#include "socket/ofxNNGNode.h"
#include "detail/apply.h"

namespace ofxNNG {
class Surveyor : public Node
{
public:
	struct Settings {
		Settings(){}
		int max_queue=16;
		nng_duration timeout_milliseconds=NNG_DURATION_DEFAULT;
		bool allow_callback_from_other_thread=false;
	};
	bool setup(const Settings &s=Settings()) {
		int result;
		result = nng_surveyor0_open(&socket_);
		if(result != 0) {
			ofLogError("ofxNNGSurveyor") << "failed to open socket; " << nng_strerror(result);
			return false;
		}
		result = nng_mtx_alloc(&work_mtx_);
		if(result != 0) {
			ofLogError("ofxNNGSurveyor") << "failed to create mutex; " << nng_strerror(result);
			return false;
		}
		result = nng_mtx_alloc(&callback_mtx_);
		if(result != 0) {
			ofLogError("ofxNNGSurveyor") << "failed to create mutex; " << nng_strerror(result);
			return false;
		}
		timeout_ = s.timeout_milliseconds;
		setEnabledAutoUpdate(!s.allow_callback_from_other_thread);
		work_.initialize(s.max_queue, &Surveyor::async, this);
		return true;
	}
	template<typename ...Ref>
	bool send(Message msg, Ref &...refs) {
		return sendImpl(msg, [&refs...](Message msg) {
			msg.to(refs...);
		});
	}
	template<typename ...Args, typename F>
	auto send(Message msg, F &&func)
	-> decltype(func(declval<Args>()...), bool()) {
		return sendImpl(msg, [func](Message msg) {
			apply<Args...>(func, msg);
		});
	}
private:
	aio::WorkPool work_;
	std::map<int, std::function<void(Message)>> callback_;
	nng_mtx *work_mtx_, *callback_mtx_;
	using AsyncWork = std::pair<int, Message>;
	ofThreadChannel<AsyncWork> channel_;
	nng_duration timeout_;
	
	static void async(void *arg) {
		auto work = (aio::Work*)arg;
		auto me = (Surveyor*)work->userdata;
		switch(work->state) {
			case aio::SEND: {
				auto result = nng_aio_result(work->aio);
				if(result != 0) {
					ofLogError("ofxNNGSurveyor") << "failed to send message; " << nng_strerror(result);
					nng_mtx_lock(me->work_mtx_);
					work->release();
					nng_mtx_unlock(me->work_mtx_);
					break;
				}
				work->state = aio::RECV;
				nng_ctx_recv(work->ctx, work->aio);
			}	break;
			case aio::RECV: {
				auto result = nng_aio_result(work->aio);
				if(result != 0) {
					if(result == NNG_ETIMEDOUT) {
						ofLogVerbose("ofxNNGSurveyor") << nng_strerror(result);
					}
					else {
						ofLogError("ofxNNGSurveyor") << "failed to receive message; " << nng_strerror(result);
					}
					nng_mtx_lock(me->work_mtx_);
					work->release();
					nng_mtx_unlock(me->work_mtx_);
					return;
				}
				AsyncWork aw{nng_ctx_id(work->ctx), nng_aio_get_msg(work->aio)};
				if(me->isEnabledAutoUpdate()) {
					me->channel_.send(std::move(aw));
				}
				else {
					me->onReceiveReply(std::move(aw));
				}
				nng_ctx_recv(work->ctx, work->aio);
			}	break;
		}
	}
	void update() {
		AsyncWork work;
		while(channel_.tryReceive(work)) {
			onReceiveReply(std::move(work));
		}
	}
	void onReceiveReply(AsyncWork work) {
		nng_mtx_lock(callback_mtx_);
		callback_[work.first](std::move(work.second));
		nng_mtx_unlock(callback_mtx_);
	}
	bool sendImpl(Message& msg, std::function<void(Message)> func) {
		aio::Work *work = nullptr;
		nng_mtx_lock(work_mtx_);
		work = work_.getUnused();
		nng_mtx_unlock(work_mtx_);
		if(!work) {
			ofLogWarning("ofxNNGSurveyor") << "no unused work";
			return false;
		}
		nng_ctx_open(&work->ctx, socket_);
		nng_aio_set_msg(work->aio, msg);
		nng_mtx_lock(callback_mtx_);
		callback_[nng_ctx_id(work->ctx)] = func;
		nng_mtx_unlock(callback_mtx_);
		nng_aio_set_timeout(work->aio, timeout_);
		work->state = aio::SEND;
		nng_ctx_send(work->ctx, work->aio);
		msg.setSentFlag();
		return true;
	}
};
}
