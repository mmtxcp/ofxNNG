#include "socket/ofxNNGReq.h"
#include "socket/ofxNNGRep.h"
#include "socket/ofxNNGPull.h"
#include "socket/ofxNNGPush.h"
#include "socket/ofxNNGPair.h"
#include "socket/ofxNNGPub.h"
#include "socket/ofxNNGSub.h"
#include "socket/ofxNNGRespondent.h"
#include "socket/ofxNNGSurveyor.h"
#include "socket/ofxNNGBus.h"
#include "util/Serialize.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace ofxNNG;
int x, y;
int k;
namespace ofxNNG {
	struct UserType {
		std::string name;
		std::string val;
		OFX_NNG_MEMBER_CONVERTER(name, val);
		// if you know this class is memcpy-able, please notify ofxNNG.
		//OFX_NNG_NOTIFY_TO_USE_MEMCPY_MEMBER
	};
}
// or if the class is third-party that you cannot add the macro in the scope, still you can notify
//OFX_NNG_NOTIFY_TO_USE_MEMCPY(UserType);

void main()
{
#pragma mark reqrep
	ofxNNG::Req req_;
	ofxNNG::Rep rep_;
	std::vector<std::thread> threads;
	{
		ofLogNotice("=================reqrep====================");
		
		Rep::Settings reps;
		reps.allow_callback_from_other_thread = true;
		reps.max_queue = 16;
		rep_.setup(reps);
		// setup with a callback function that returns std::pair<bool, anything>.
		// if the first bool was true, the second arg will be sent to the peer. else nothing is sent.
		rep_.setCallback<int, std::string>([](int index, const std::string& message) {
			//return std::make_pair(index == ' ', std::to_string(index) + " " + message);
			ofLogNotice("rep_ response") << message;
			return std::make_pair(true, "test: " + std::to_string(index) + " " + message);
			//return std::make_pair(true, std::to_string(index) + " " + message);
			});
		//rep_.createListener("tcp://127.0.0.1:9000")->start();
		rep_.createListener("ipc:///temp/pp1")->start();
		/*threads.emplace_back([&rep_] {
			
			ofEvents().notifyUpdate();
			std::this_thread::sleep_for(std::chrono::seconds(2));
			});*/
		Req::Settings reqs;
		reqs.max_queue = 16;
		reqs.timeout_milliseconds = 1000;
		reqs.allow_callback_from_other_thread = true;
		req_.setup(reqs);
		//req_.createDialer("tcp://127.0.0.1:9000")->start();
		req_.createDialer("ipc:///temp/pp1")->start();

		int i = 0;
		//while (++i < 1000)
		{
			/*ofxNNG::Message msg;
			msg.append(i);
			msg.append("pressed key");*/
			req_.send<std::string>({i,"pressed key" }, [](const std::string& response) {
				ofLogNotice("got response") << response;
				});
			std::this_thread::sleep_for(std::chrono::microseconds(200));
			//ofEvents().notifyUpdate();

			/*threads.emplace_back([&req_, i,&msg] {
				req_.send<std::string>(msg, [](const std::string& response) {
					ofLogNotice("got response") << response;
					});
				ofEvents().notifyUpdate();
			//std::this_thread::sleep_for(std::chrono::seconds(2));
			});*/
		}
		/*for (auto& thr : threads)
		{
			thr.join();
		}*/
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
//#pragma mark pubsub
//	ofxNNG::Pub pub_;
//	std::vector<std::shared_ptr<ofxNNG::Sub>> sub_;
//	{
//		ofLogNotice("=================pubsub=================");
//		
//		
//		pub_.setup();
//		pub_.createListener("inproc://pub")->start();
//
//		sub_.resize(4);
//		for (auto& s : sub_) {
//			s = std::make_shared<ofxNNG::Sub>();
//			s->setup();
//			// subscribing specific topic
//			// template argument type is which incoming msg will be converted to.
//			s->subscribe<std::string>("str", [](const std::string& message) {
//				ofLogNotice("str") << message;
//				});
//			
//			// topics can be anything other than string.
//			// this feature may be used for receiving a kind of raw binary data.
//			unsigned char soi[] = { 0xFF,0xD8 };
//			s->subscribe<Message>({ soi,2 }, [](const Message& msg) {
//				ofLogNotice("soi") << "jpg data";
//				});
//			// also you can receive by references
//			s->subscribe("position", x, y);
//
//			// you can subscribe all message by subscribing with empty topic.
//			// but subscriber can't know how long is the topic the sender expected. 
//			// so if you want to separate them you need some external rules.
//	//		s->subscribe("", something);
//
//			s->createDialer("inproc://pub")->start();
//		}
//
//		// rough patch for waiting for connection
//		ofSleepMillis(300);
//
//		pub_.send("str", "string message");
//		uint8_t binary_topic[] = { 0,1,2,3 };
//		pub_.send(binary_topic, sizeof(binary_topic), "binary topic");
//		uint8_t jpeg_data[] = { 0xFF, 0xD8, 0,0,0,0,0,0 };
//		pub_.send(jpeg_data, sizeof(2), jpeg_data);
//		pub_.send("position", { 110,120 });
//	}
//	std::this_thread::sleep_for(std::chrono::seconds(2));
//#pragma mark pair
//	ofxNNG::Pair node0_, node1_, node2_;
//	{
//		ofLogNotice("=================pair====================");
//		
//		using std::string;
//		using std::cout;
//		using std::endl;
//
//		ofxNNG::Pair::Settings pairs;
//		pairs.polyamorous_mode = true;
//		pairs.allow_callback_from_other_thread = false;
//
//		// connection diagram; node1 <--> node0 <--> node2
//		node0_.setup(pairs);
//		node0_.setCallback<string, string>([&node0_](const string& str1, const string& str2, nng_pipe pipe) {
//			/*const string& str1 = ut.name;
//			const string& str2 = ut.val;*/
//			cout << "node0: got a message from pipe:" << nng_pipe_id(pipe) << endl;
//			cout << str1 << str2 << endl;
//			// node0 is connected to both nodes but it can specify which to send by suggesting a pipe
//			node0_.send("this is a reply from node0", false, pipe);
//			});
//		node1_.setup(pairs);
//		node1_.setCallback<string>([](const string& str, nng_pipe pipe) {
//			cout << "node1: got a reply from pipe:" << nng_pipe_id(pipe) << endl;
//			cout << str << endl;
//			});
//		node2_.setup(pairs);
//		node2_.setCallback<string>([](const string& str, nng_pipe pipe) {
//			cout << "node2: got a reply from pipe:" << nng_pipe_id(pipe) << endl;
//			cout << str << endl;
//			});
//		string url = "inproc://pair2";
//		node0_.createListener(url)->start();
//		node1_.createDialer(url)->start();
//		//node2_.createDialer(url)->start();
//		//UserType ut = { "this is a message", " from node1" };
//		std::thread t([&node1_] {
//			node1_.send({ "this is a message", " from node1" });
//			std::this_thread::sleep_for(std::chrono::seconds(1));
//			});
//		t.join();
//		/*std::thread t2([&node2_] {
//			node2_.send({ "this is a message", " from node1" });
//			std::this_thread::sleep_for(std::chrono::seconds(1));
//			});
//		t2.join();*/
//		/*node1_.send({ "this is a message", " from node2" });
//		node2_.send({ "this is a message", " from node2" });*/
//	}
//	std::this_thread::sleep_for(std::chrono::seconds(1));
//#pragma mark pushpull
//	ofxNNG::Push push_;
//	ofxNNG::Pull pull_;
//	{
//		ofLogNotice("=================pushpull=================");
//		
//		push_.setup();
//		push_.createListener("inproc://push")->start();
//
//		pull_.setup();
//		//pull_.setCallback(k);
//		pull_.setCallback<int>([](int k) {
//			cout << " got a reply k:"  << to_string(k) << endl;
//			});
//		pull_.createDialer("inproc://push")->start();
//		std::thread t2([&push_] {
//			push_.send(45);
//			std::this_thread::sleep_for(std::chrono::seconds(1));
//			});
//		t2.join();
//		//push_.send(45);
//	}
//	std::this_thread::sleep_for(std::chrono::seconds(1));
//#pragma mark bus
//	std::vector<std::shared_ptr<ofxNNG::Bus>> bus_;
//	std::vector<std::thread> threads;
//	{
//		ofLogNotice("=================bus=================");
//		
//		 4 buses making all-to-all mesh connection 
//		bus_.resize(4);
//		for (int i = 0; i < bus_.size(); ++i) {
//			auto& b = bus_[i];
//			b = std::make_shared<Bus>();
//			b->setup();
//			b->setCallback<std::string, int>([i](const std::string& str, int index) {
//				ofLogNotice("node " + ofToString(i) + " receive from " + ofToString(index)) << str;
//				});
//			std::string recv_url = "inproc://bus" + ofToString(i);
//			b->createListener(recv_url)->start();
//			for (int j = i + 1; j < bus_.size(); ++j) {
//				std::string send_url = "inproc://bus" + ofToString(j);
//				b->createDialer(send_url)->start();
//			}
//		}
//		int key = '1';
//		int index = key - '1';
//		while (index<4)
//		{
//			if (index >= 0 && index < bus_.size()) {
//				/*std::thread t2([&bus_,&key,&index] {
//					bus_[key - '1']->send({ "message from node " + ofToString(index), index });
//					ofLogNotice("from node " + ofToString(index) + " send");
//					std::this_thread::sleep_for(std::chrono::seconds(1));
//					});*/
//				threads.emplace_back([&bus_, key, index] {
//					bus_[key - '1']->send({ "message from node " + ofToString(index), index });
//					ofLogNotice("from node " + ofToString(index) + " send");
//					std::this_thread::sleep_for(std::chrono::seconds(1));
//					});
//				std::this_thread::sleep_for(std::chrono::seconds(1));
//				/*bus_[key - '1']->send({ "message from node " + ofToString(index), index });
//				ofLogNotice("from node " + ofToString(index) + " send");*/
//			}
//			++key;
//			index = key - '1';
//		}
//		
//		
//	}
//	for (auto& thr :threads)
//	{
//		thr.join();
//	}
//	std::this_thread::sleep_for(std::chrono::seconds(10));
//#pragma mark survery
//	ofxNNG::Surveyor survey_;
//	{
//		ofLogNotice("=================survey=================");
//		
//		std::vector<std::shared_ptr<ofxNNG::Respondent>> respond_;
//		survey_.setup();
//		survey_.createListener("inproc://survey")->start();
//
//		const vector<string> names{
//			"apple",
//			"banana",
//			"cylinder",
//			"device",
//			"bang",
//			"circle"
//		};
//		for (auto&& n : names) {
//			auto r = std::make_shared<ofxNNG::Respondent>();
//			r->setup();
//			r->setCallback<char, string>([n](char ch, const string& message) {
//				return std::make_pair(ch == n[0], n + " is here!");
//				});
//			r->createDialer("inproc://survey")->start();
//			respond_.emplace_back(r);
//
//			int key = 'a';
//			char ch = key;
//			ofLogNotice("survey") << "is there anyone who's name starts with:" << ch;
//			survey_.send<string>({ ch, "pressed" }, [](const string& response) {
//				ofLogNotice("renponse") << response;
//				});
//		}
//	}

	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "Awake after 5 seconds!" << std::endl;
	
}
