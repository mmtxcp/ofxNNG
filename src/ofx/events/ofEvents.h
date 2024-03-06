#pragma once

#define GLM_FORCE_CTOR_INIT
#define GLM_ENABLE_EXPERIMENTAL

#include "ofx/events/ofEventUtils.h"

#include "ofx/utils/ofTimer.h"
#include <set>



//-----------------------------------------------
// event arguments, this are used in oF to pass
// the data when notifying events

class ofEventArgs { };


class ofMessage : public ofEventArgs {
public:
	ofMessage(std::string msg) {
		message = msg;
	}
	std::string message;
};

enum ofTimeMode {
	System,
	FixedRate,
	Filtered,
};

class ofCoreEvents {
public:
	ofCoreEvents();
	ofEvent<ofEventArgs> setup;
	ofEvent<ofEventArgs> update;
	ofEvent<ofEventArgs> exit;

	

	ofEvent<ofMessage> messageEvent;
	
	ofEvent<uint32_t> charEvent;

	void disable();
	void enable();

	void setTimeModeSystem();
	void setTimeModeFixedRate(uint64_t nanosecsPerFrame);
	void setTimeModeFiltered(float alpha);
	ofTimeMode getTimeMode() const;
	//  event notification only for internal OF use
	bool notifySetup();
	bool notifyUpdate();
	bool notifyExit();
	

private:
	ofTimer timer;
	enum TimeMode {
		System,
		FixedRate,
		Filtered,
	} timeMode
		= System;
	std::chrono::nanoseconds fixedRateTimeNanos;
};

bool ofSendMessage(ofMessage msg);
bool ofSendMessage(std::string messageString);

ofCoreEvents & ofEvents();


template <class ListenerClass>
void ofRegisterGetMessages(ListenerClass * listener, int prio = OF_EVENT_ORDER_AFTER_APP) {
	ofAddListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage, prio);
}

template <class ListenerClass>
void ofUnregisterGetMessages(ListenerClass * listener, int prio = OF_EVENT_ORDER_AFTER_APP) {
	ofRemoveListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage, prio);
}

template <class ListenerClass>
void ofRegisterUpdate(ListenerClass* listener, int prio = OF_EVENT_ORDER_AFTER_APP) {
	ofAddListener(ofEvents().update, listener, &ListenerClass::update, prio);
}

template <class ListenerClass>
void ofUnregisterUpdate(ListenerClass* listener, int prio = OF_EVENT_ORDER_AFTER_APP) {
	ofRemoveListener(ofEvents().update, listener, &ListenerClass::update, prio);
}
