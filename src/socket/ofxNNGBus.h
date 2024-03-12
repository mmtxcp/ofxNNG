﻿#pragma once

#include "nng.h"
#include "protocol/bus0/bus.h"
#include "socket/ofxNNGNode.h"
#include "message/ofxNNGMessage.h"
#include "detail/apply.h"

namespace ofxNNG {
class Bus : public Node
{
public:
	struct Settings {
		Settings(){}
		bool allow_callback_from_other_thread=false;
	};
	bool setup(const Settings &s=Settings()) {
		int result;
		result = nng_bus_open(&socket_);
		if(result != 0) {
			ofLogError("ofxNNGBus") << "failed to open socket; " << nng_strerror(result);
			return false;
		}
		setEnabledAutoUpdate(!s.allow_callback_from_other_thread);
		nng_aio_alloc(&aio_, &Bus::receive, this);
		nng_recv_aio(socket_, aio_);
		return true;
	}
	template<typename ...Args, typename F>
	auto setCallback(F &&func)
	-> decltype(func(declval<Args>()...), void()) {
		callback_ = [func](Message msg) {
			apply<Args...>(func, msg);
		};
	}
	template<typename ...Ref>
	void setCallback(Ref &...refs) {
        callback_ = defaultMsgConvFun;
		/*callback_ = [&refs...](Message msg) {
			msg.to(refs...);
		};*/
	}
	bool send(Message msg, bool blocking=false) {
		int result;
		int flags = 0;
		if(!blocking) flags |= NNG_FLAG_NONBLOCK;
		result = nng_sendmsg(socket_, msg, flags);
		if(result != 0) {
			ofLogError("ofxNNGBus") << "failed to send message; " << nng_strerror(result);
			return false;
		}
		msg.setSentFlag();
		return true;
	}
	~Bus() {
		if(aio_) nng_aio_free(aio_);
	}
private:
	nng_aio *aio_;
	std::function<void(Message)> callback_;
	ofThreadChannel<Message> channel_;
	static void receive(void *arg) {
		auto me = (Bus*)arg;
		auto result = nng_aio_result(me->aio_);
		if(result != 0) {
			ofLogError("ofxNNGBus") << "failed to receive message; " << nng_strerror(result);
			return;
		}
		Message msg(nng_aio_get_msg(me->aio_));
		if(me->isEnabledAutoUpdate()) {
			me->channel_.send(std::move(msg));
		}
		else {
			me->callback_(std::move(msg));
		}
		nng_recv_aio(me->socket_, me->aio_);
	}
	void update() {
		Message msg;
		while(channel_.tryReceive(msg)) {
			callback_(std::move(msg));
		}
	}
};
}
