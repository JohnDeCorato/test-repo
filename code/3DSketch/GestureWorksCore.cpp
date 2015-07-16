////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    GestureWorksCore.cpp
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#include "GestureWorksCore.h"
#include <QLibrary>
#include <QDebug>


typedef void (*initializeGestureworksType)(int, int);
typedef void (*resizeScreenType)(int, int);
typedef void (*processFrameType)(void);
typedef gwc::PointEventArray* (*consumePointEventsType)(void);
typedef gwc::IntermediateGestureEventArray* (*consumeGestureEventsType)();
typedef bool (*loadGMLType)(char*);
typedef bool (*addGestureType)(char*,char*);
typedef bool (*addGestureSetType)(char*,char*);
typedef bool (*removeGestureType)(char*, char*);
typedef bool (*removeGestureSetType)(char*, char*);
typedef bool (*enableGestureType)(char*, char*);
typedef bool (*disableGestureType)(char*, char*);
typedef bool (*registerWindowForTouchByNameType)(char*);
typedef void (*registerTouchObjectType)(char*);
typedef bool (*deregisterTouchObjectType)(char*);
typedef bool (*assignTouchPointType)(char*, int);
typedef void (*addEventType)(gwc::touchpoint);

static disableGestureType _disableGesture(0);
static addGestureType _addGesture(0);
static enableGestureType _enableGesture(0);
static loadGMLType _loadGML(0);
static processFrameType _processFrame(0);
static initializeGestureworksType _initializeGestureWorks(0);
static resizeScreenType _resizeScreen(0);
static consumePointEventsType _consumePointEvents(0);
static registerWindowForTouchByNameType _registerWindowForTouchByName(0);
static registerTouchObjectType _registerTouchObject(0);
static assignTouchPointType _assignTouchPoint(0);
static consumeGestureEventsType _consumeGestureEvents(0);
static deregisterTouchObjectType _deregisterTouchObject(0);
static addGestureSetType _addGestureSet(0);
static removeGestureType _removeGesture(0);
static removeGestureSetType _removeGestureSet(0);
static addEventType _addEvent(0);

static HINSTANCE gwcDLL(0);

int loadGestureWorks(QString dll_path) {

	
	int success = 0;

    //gwcDLL = LoadLibraryA(dll_path.c_str());
    QLibrary gwcDLL(dll_path);

    qDebug() << gwcDLL.fileName();
    qDebug() << gwcDLL.load();
	
    if(gwcDLL.isLoaded()) {
        _processFrame = (processFrameType)gwcDLL.resolve("processFrame");
        _initializeGestureWorks = (initializeGestureworksType) gwcDLL.resolve("initializeGestureWorks");
        _resizeScreen = (resizeScreenType) gwcDLL.resolve("resizeScreen");
        _consumePointEvents = (consumePointEventsType) gwcDLL.resolve("consumePointEvents");
        _consumeGestureEvents = (consumeGestureEventsType) gwcDLL.resolve("consumeGestureEvents");
        _registerWindowForTouchByName = (registerWindowForTouchByNameType) gwcDLL.resolve("registerWindowForTouchByName");
        _registerTouchObject = (registerTouchObjectType) gwcDLL.resolve("registerTouchObject");
        _assignTouchPoint = (assignTouchPointType) gwcDLL.resolve("addTouchPoint");
        _loadGML = (loadGMLType) gwcDLL.resolve("loadGML");
        _enableGesture = (enableGestureType) gwcDLL.resolve("enableGesture");
        _disableGesture = (disableGestureType) gwcDLL.resolve("disableGesture");
        _addGesture = (addGestureType) gwcDLL.resolve("addGesture");
        _deregisterTouchObject = (deregisterTouchObjectType) gwcDLL.resolve("deregisterTouchObject");
        _addGestureSet = (addGestureSetType) gwcDLL.resolve("addGestureSet");
        _removeGesture = (removeGestureType) gwcDLL.resolve("removeGesture");
        _removeGestureSet = (removeGestureSetType) gwcDLL.resolve("removeGestureSet");
        _addEvent = (addEventType) gwcDLL.resolve("addEvent");

		if(
			!_processFrame ||
			!_initializeGestureWorks ||
			!_resizeScreen ||
			!_consumePointEvents ||
			!_consumeGestureEvents ||
			!_registerWindowForTouchByName ||
			!_registerTouchObject ||
			!_assignTouchPoint ||
			!_loadGML ||
			!_enableGesture ||
			!_disableGesture ||
			!_addGesture ||
			!_addGestureSet ||
			!_removeGesture ||
			!_removeGestureSet ||
			!_deregisterTouchObject ||
			!_addEvent

            ) {success = 2; }

		
	} else { 
		success = 1;
	}
	
	return success;
}


void initializeGestureWorks(int screen_width, int screen_height) {
	_initializeGestureWorks(screen_width, screen_height);
}
void resizeScreen(int screen_width, int screen_height) {
	_resizeScreen(screen_width, screen_height);
}
void processFrame(void) {
	_processFrame();
}
std::vector<gwc::PointEvent> consumePointEvents(void) {
	gwc::PointEventArray* point_events = _consumePointEvents();
	std::vector<gwc::PointEvent> events;
	for(int i = 0; i < point_events->size; i++) {
		events.push_back(point_events->events[i]);
	}
	return events;
}
std::vector<gwc::GestureEvent> consumeGestureEvents(void) {
	gwc::IntermediateGestureEventArray* gesture_events = _consumeGestureEvents();
	std::vector<gwc::GestureEvent> events;
	for(int i = 0; i < gesture_events->size; i++) {
		events.push_back(gwc::GestureEvent(gesture_events->events[i]));
	}
	return events;
}
bool registerWindowForTouchByName(std::string window_name) {
	return _registerWindowForTouchByName((char*)window_name.c_str());
}
void registerTouchObject(std::string object_id) {
	return _registerTouchObject((char*)object_id.c_str());
}
bool assignTouchPoint(std::string object_id, int point_id) {
	return _assignTouchPoint((char*)object_id.c_str(), point_id);
}
bool loadGML(std::string file_name) {
	return _loadGML((char*)file_name.c_str());
}
bool addGesture(std::string object_id, std::string gesture_id) {
	return _addGesture((char*)object_id.c_str(), (char*)gesture_id.c_str());
}
bool addGestureSet(std::string object_id, std::string set_name) {
	return _addGestureSet((char*)object_id.c_str(), (char*)set_name.c_str());
}
bool removeGesture(std::string object_id, std::string gesture_id) {
	return _removeGesture((char*)object_id.c_str(), (char*)gesture_id.c_str());
}
bool removeGestureSet(std::string object_id, std::string set_name) {
	return _removeGestureSet((char*)object_id.c_str(), (char*)set_name.c_str());
}
bool enableGesture(std::string object_id, std::string gesture_id) {
	return _enableGesture((char*)object_id.c_str(), (char*)gesture_id.c_str());
}
bool disableGesture(std::string object_id, std::string gesture_id) {
	return _disableGesture((char*)object_id.c_str(), (char*)gesture_id.c_str());
}
bool deregisterTouchObject(std::string object_id) {
	return _deregisterTouchObject((char*)object_id.c_str());
}
void addEvent(gwc::touchpoint touch_event) {
	_addEvent(touch_event);
}
