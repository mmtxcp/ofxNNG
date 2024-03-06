#pragma once

#include <stddef.h>
#include "nng.h"
#include "protocol/pipeline0/push.h"
#include "util/ASyncWork.h"
#include "message/ofxNNGMessage.h"
#include "socket/ofxNNGNode.h"

namespace ofxNNG {
class Push : public Node
{
public:
	struct Settings {
		Settings(){}
	};
	bool setup(const Settings &s=Settings()) {
		int result;
		result = nng_push0_open(&socket_);
		if(result != 0) {
			ofLogError("ofxNNGPush") << "failed to open socket; " << nng_strerror(result);
			return false;
		}
		return true;
	}
	bool send(Message msg, bool blocking=false) {
		int result;
		int flags = 0;
		if(!blocking) flags |= NNG_FLAG_NONBLOCK;
		result = nng_sendmsg(socket_, msg, flags);
		if(result != 0) {
			ofLogError("ofxNNGPush") << "failed to send message; " << nng_strerror(result);
			return false;
		}
		msg.setSentFlag();
		return true;
	}
};
}
