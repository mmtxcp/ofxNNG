#pragma once

#include "message/ofxNNGMessage.h"
#include "ofx/utils/ofFileUtils.h"

namespace ofxNNG {
	struct Serialize : Message {
		using Message::Message;
		operator ofBuffer() const {
			return ofBuffer(static_cast<const char*>(data()), size());
		}
		//path绝对路径
		void write(const std::string &path) {
			//ofBufferToFile(path, *this);
			ofBufferToFile(path, ofBuffer(static_cast<const char*>(data()), size()));
		}
		/*void write(const of::filesystem::path& path) {
			ofBufferToFile(path.string(), *this);
		}*/
	};
	struct Deserialize : Message {
		Deserialize(const ofBuffer &data):Message() {
			appendData(data.getData(), data.size());
		}
		//path绝对路径
		Deserialize(const std::string&path):Deserialize(ofBufferFromFile(path)) {}
		//Deserialize(const of::filesystem::path& path) :Deserialize(ofBufferFromFile(path.string())) {}
		using Message::to;
		using Message::get;
	};
}
