
#include "ofx/events/ofEvents.h"
#include "ofx/utils/ofLog.h"

static ofEventArgs voidEventArgs;

ofCoreEvents::ofCoreEvents() {
}

//------------------------------------------
void ofCoreEvents::disable() {
	setup.disable();
	update.disable();
	exit.disable();
	
	messageEvent.disable();
	
}

//------------------------------------------
void ofCoreEvents::enable() {
	setup.enable();
	update.enable();
	exit.enable();
	messageEvent.enable();
}

void ofCoreEvents::setTimeModeSystem() {
	timeMode = System;
}

ofTimeMode ofCoreEvents::getTimeMode() const {
	return ofTimeMode(timeMode);
}

void ofCoreEvents::setTimeModeFixedRate(uint64_t nanosecsPerFrame) {
	timeMode = FixedRate;
	fixedRateTimeNanos = std::chrono::nanoseconds(nanosecsPerFrame);
}

void ofCoreEvents::setTimeModeFiltered(float alpha) {
	timeMode = Filtered;
}



//------------------------------------------
bool ofCoreEvents::notifySetup() {
	return ofNotifyEvent(setup, voidEventArgs);
}


//------------------------------------------
bool ofCoreEvents::notifyUpdate() {
	return ofNotifyEvent(update, voidEventArgs);
}


//------------------------------------------
bool ofCoreEvents::notifyExit() {
	return ofNotifyEvent(exit, voidEventArgs);
}

//------------------------------------------
bool ofSendMessage(ofMessage msg) {
	return ofNotifyEvent(ofEvents().messageEvent, msg);
}

//------------------------------------------
bool ofSendMessage(std::string messageString) {
	ofMessage msg(messageString);
	return ofSendMessage(msg);
}

ofCoreEvents& ofEvents()
{
	static auto* noopEvents = new ofCoreEvents();
	return *noopEvents;
}

//------------------------------------------
namespace of {
namespace priv {
std::atomic<uint_fast64_t> StdFunctionId::nextId;

AbstractEventToken::~AbstractEventToken() { }

BaseFunctionId::~BaseFunctionId() { }

StdFunctionId::~StdFunctionId() { }
}
}
