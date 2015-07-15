////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    Hello LEAP.cpp
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ofMain.h"
#include "Hello LEAP.h"

//This vector keeps track of the points to be displayed on screen
std::map<int,gwc::Point> active_points;

CameraListener leap_listener;


//Thread function necessary to continuously listen for LEAP events
DWORD LEAP_thread(LPVOID lpdwThreadParam) {
	Leap::Controller leap_controller(leap_listener);
	while(1) {}
	return 0;
}

//--------------------------------------------------------------
void helloLEAP::setup(){

	ofSetFullscreen(true);

	last_tick_count = GetTickCount();

	active_points = std::map<int,gwc::Point>();



	if(loadGestureWorks("GestureWorksCore.dll")) { 
		std::cout << "Error loading gestureworks dll" << std::endl; 
		exit(); 
	}
	if(!loadGML("basic_manipulation.gml")) { 
		std::cout << "Could not find gml file" << std::endl; 
		exit(); 
	}

	initializeGestureWorks(1920,1080);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&LEAP_thread, 0, 0, 0);
}

//This function is the last to get called every cycle, and is responsible
//for generating all screen artifacts
void helloLEAP::draw(){

	ofBackground(ofColor(255,255,255));
	ofSetColor(238, 57, 135, 100);
	//For each active point
	for(std::map<int,gwc::Point>::iterator points_it = active_points.begin(); points_it != active_points.end(); points_it++)
	{
		float x = points_it->second.getX();
		float y = points_it->second.getY();

		//Generate a circle with a 50-pixel radius at this point's location
		ofFill();
		ofCircle(ofPoint(x, y, 0), 20);
		ofNoFill();
		ofSetLineWidth(2);
		ofCircle(ofPoint(x, y, 0), 30);


		//Generate a stringstream for each value with which we're concerned
		std::stringstream xvals; xvals << x;
		std::stringstream yvals; yvals << y;
		std::stringstream ids; ids << points_it->first;

		//Annotate the circle we just drew with the id, x and y values for the corresponding point
		ofDrawBitmapString("ID: " + ids.str() + " X: " + xvals.str() + " Y: " + yvals.str(), x + 40, y - 40, 0);
	}
}

//The update function gets called once per cycle prior to the draw function
void helloLEAP::update(){

	//Updating once every 16 milliseconds corresponds to a rate of approximately 62.5 frames per second
	if(GetTickCount() - last_tick_count < 16) {
		return;
	} else { last_tick_count = GetTickCount(); }
  
	processFrame();

	std::vector<gwc::PointEvent> point_events = consumePointEvents();
	
	for(std::vector<gwc::PointEvent>::iterator event_it = point_events.begin(); event_it != point_events.end(); event_it++) {
		switch(event_it->status) {
			case gwc::TOUCHADDED:
			case gwc::TOUCHUPDATE:
				//If the point is being added, this will place it in our point map; the same line of code will update the point's
				//position if it's already present, so we can use this command to handle new points as well as point updates
				active_points[event_it->point_id] = gwc::Point(event_it->position.getX(),event_it->position.getY());
				break;
			case gwc::TOUCHREMOVED:
				//Remove the point from the map
				active_points.erase(event_it->point_id);
				break;
		}
	}
}
