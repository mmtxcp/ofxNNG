#pragma once

#include <stddef.h>
#include "nng.h"
#include "protocol/pubsub0/pub.h"
#include "socket/ofxNNGNode.h"
//#include "ofx/utils/ofLog.h"
#include "message/ofxNNGMessage.h"

namespace ofxNNG {
class Pub : public Node
{
public:
	struct Settings {
		Settings(){}
	};
	bool setup(const Settings &s=Settings()) {
		int result;
		result = nng_pub0_open(&socket_);
		if(result != 0) {
			ofLogError("ofxNNGPub") << "failed to open socket; " << nng_strerror(result);
			return false;
		}
		return true;
	}
	bool send(Message msg) {
		int result;
		result = nng_sendmsg(socket_, msg, 0);
		if(result != 0) {
			ofLogError("ofxNNGPub") << "failed to send message; " << nng_strerror(result);
			return false;
		}
		msg.setSentFlag();
		return true;
	}
	bool send(const std::string &topic, Message msg) {
		return send(topic.data(), topic.size(), std::move(msg));
	}
	bool send(const void *topic_data, std::size_t topic_size, Message msg) {
		msg.prependData(topic_data, topic_size);
		return send(std::move(msg));
	}
};
}

