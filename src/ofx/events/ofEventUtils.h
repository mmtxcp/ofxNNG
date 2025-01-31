#pragma once

#include "ofx/events/ofEvent.h"
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------
/// register any method of any class to an event.
///
/// the method must provide one of the following
/// signatures:
///     void method(ArgumentsType & args)
///     void method(const void * sender, ArgumentsType &args)
/// ie:
///     ofAddListener(addon.newIntEvent, this, &Class::method)

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofAddListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofAddListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
    event.add(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType>
void ofAddListener(EventType & event, void (*listenerFunction)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
    event.add(listenerFunction, prio);
}

template <class EventType,typename ArgumentsType>
void ofAddListener(EventType & event, void (*listenerFunction)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
    event.add(listenerFunction, prio);
}

inline void ofAddListener(ofEvent<void> & event, void (*listenerFunction)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
    event.add(listenerFunction, prio);
}

inline void ofAddListener(ofEvent<void> & event, void (*listenerFunction)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
    event.add(listenerFunction, prio);
}

template <class EventType,typename ArgumentsType>
void ofAddListener(EventType & event, bool (*listenerFunction)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
    event.add(listenerFunction, prio);
}

template <class EventType,typename ArgumentsType>
void ofAddListener(EventType & event, bool (*listenerFunction)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(*listenerFunction, prio);
    event.add(*listenerFunction, prio);
}

inline void ofAddListener(ofEvent<void> & event, bool (*listenerFunction)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(*listenerFunction, prio);
    event.add(*listenerFunction, prio);
}

inline void ofAddListener(ofEvent<void> & event, bool (*listenerFunction)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
    event.add(listenerFunction, prio);
}
//----------------------------------------------------
/// unregister any method of any class to an event.
///
/// the method must provide one the following
/// signatures:
///     void method(ArgumentsType & args)
///     void method(const void * sender, ArgumentsType &args)
/// ie:
///     ofAddListener(addon.newIntEvent, this, &Class::method)

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType, class ListenerClass>
void ofRemoveListener(EventType & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class ListenerClass>
void ofRemoveListener(ofEvent<void> & event, ListenerClass  * listener, bool (ListenerClass::*listenerMethod)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listener, listenerMethod, prio);
}

template <class EventType,typename ArgumentsType>
void ofRemoveListener(EventType & event, void (*listenerFunction)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}

template <class EventType,typename ArgumentsType>
void ofRemoveListener(EventType & event, void (*listenerFunction)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}

inline void ofRemoveListener(ofEvent<void> & event, void (*listenerFunction)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}

inline void ofRemoveListener(ofEvent<void> & event, void (*listenerFunction)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}

template <class EventType,typename ArgumentsType>
void ofRemoveListener(EventType & event, bool (*listenerFunction)(const void*, ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}

template <class EventType,typename ArgumentsType>
void ofRemoveListener(EventType & event, bool (*listenerFunction)(ArgumentsType&), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}

inline void ofRemoveListener(ofEvent<void> & event, bool (*listenerFunction)(const void*), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}

inline void ofRemoveListener(ofEvent<void> & event, bool (*listenerFunction)(), int prio=OF_EVENT_ORDER_AFTER_APP){
    event.remove(listenerFunction, prio);
}
//----------------------------------------------------
/// notifies an event so all the registered listeners
/// get called
///
/// ie:
///	ofNotifyEvent(addon.newIntEvent, intArgument, this)
///
/// or in case there's no sender:
///	ofNotifyEvent(addon.newIntEvent, intArgument)
///
/// @returns: true in case any listener attended the event

template <class EventType,typename ArgumentsType, typename SenderType>
inline bool ofNotifyEvent(EventType & event, ArgumentsType & args, SenderType * sender){
	return event.notify(sender,args);
}

template <class EventType,typename ArgumentsType>
inline bool ofNotifyEvent(EventType & event, ArgumentsType & args){
	return event.notify(args);
}

template <class EventType, typename ArgumentsType, typename SenderType>
inline bool ofNotifyEvent(EventType & event, const ArgumentsType & args, SenderType * sender){
	return event.notify(sender,args);
}

template <class EventType,typename ArgumentsType>
inline bool ofNotifyEvent(EventType & event, const ArgumentsType & args){
	return event.notify(args);
}

template <typename SenderType>
inline bool ofNotifyEvent(ofEvent<void> & event, SenderType * sender){
	return event.notify(sender);
}

inline bool ofNotifyEvent(ofEvent<void> & event){
	return event.notify();
}

