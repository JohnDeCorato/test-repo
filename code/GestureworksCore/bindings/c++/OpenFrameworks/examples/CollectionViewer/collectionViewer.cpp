////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    collectionViewer.cpp
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ofMain.h"
#include "collectionViewer.h"
#include "MediaBoxes.h"

//--------------------------------------------------------------
void collectionViewer::setup(){
	ofBackground(255,255,255);
	window_position_x = ofGetWindowPositionX();
	window_position_y = ofGetWindowPositionY();

	background.loadImage("gw_c.png");

	ofEnableAlphaBlending();
	ofSetFullscreen(true);

	last_tick_count = GetTickCount();


		//Create objects that we'll be using
		PictureBox * boccioni = new PictureBox(this, scene_list, "boccioni", 331, 600);
		boccioni->loadImage("boccioni_600.png");
		boccioni->loadToggle("boccioni_600_reverse.png");
		boccioni->setRotation(3.1415926/4);
		boccioni->setScale(1);

		MovieBox * clock = new MovieBox(this, scene_list, "clock", 720, 404);
		clock->loadMovie("clock.mp4");
		clock->loadToggle("clock_back.png");
		clock->setX(700);
		clock->setY(300);
		clock->setScale(1);

		scene_list.push_back(boccioni);
		scene_list.push_back(clock);

		vector<MediaBox*>::iterator scene_it;


	if(loadGestureWorks("GestureWorksCore.dll")) { std::cout << "Error loading gestureworks dll" << std::endl; exit(); }
	if(!loadGML("collection_viewer_gestures.gml")) { std::cout << "Could not find gml file" << std::endl; exit(); }
	initializeGestureWorks(1920,1080);

	if( !registerWindowForTouchByName("testify") ) { std::cout << "Could not register collection viewer window for touch." << std::endl; exit(); }

	registerTouchObject("boccioni");
	registerTouchObject("boccioni.toggle");
	registerTouchObject("boccioni.close");
	registerTouchObject("clock");
	registerTouchObject("clock.play");
	registerTouchObject("clock.stop");
	registerTouchObject("clock.toggle");
	registerTouchObject("clock.close");

	//Set up and register boccioni picturebox
	scene_map["boccioni"] = (MediaBox*)boccioni;
	scene_map["boccioni.toggle"] = (MediaBox*)&boccioni->info_toggle;
	scene_map["boccioni.close"] = (MediaBox*)&boccioni->close;

	addGesture("boccioni", "n-drag");
	addGesture("boccioni", "n-rotate");
	addGesture("boccioni", "n-scale");

	addGesture("boccioni.toggle", "n-tap");
	addGesture("boccioni.close", "n-tap");

	//Set up and register clock moviebox
	scene_map["clock"] = (MediaBox*)clock;
	scene_map["clock.play"] = (MediaBox*)&clock->play;
	scene_map["clock.stop"] = (MediaBox*)&clock->stop;
	scene_map["clock.toggle"] = (MediaBox*)&clock->info_toggle;
	scene_map["clock.close"] = (MediaBox*)&clock->close;

	addGesture("clock", "n-drag");
	addGesture("clock", "n-rotate");
	addGesture("clock", "n-scale");

	addGesture("clock.play", "n-tap");
	addGesture("clock.stop", "n-tap");
	addGesture("clock.toggle", "n-tap");
	addGesture("clock.close", "n-tap");


}





//--------------------------------------------------------------
void collectionViewer::keyPressed  (int key){
    
}

//--------------------------------------------------------------
void collectionViewer::keyReleased(int key){

}

//--------------------------------------------------------------
void collectionViewer::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void collectionViewer::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void collectionViewer::mousePressed(int x, int y, int button){

}


//--------------------------------------------------------------
void collectionViewer::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void collectionViewer::windowResized(int w, int h){

}

//--------------------------------------------------------------
void collectionViewer::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void collectionViewer::dragEvent(ofDragInfo dragInfo){ 

}
	
//--------------------------------------------------------------
void collectionViewer::draw(){

	background.draw(0, 0, 1920, 1080);

	vector<MediaBox*>::iterator scene_it;
	for(scene_it = scene_list.begin(); scene_it < scene_list.end(); scene_it++) {
		(*scene_it)->draw();
	}
}


void collectionViewer::update(){

	if(GetTickCount() - last_tick_count < 16) {
		return;
	} else { last_tick_count = GetTickCount(); }
    vector<MediaBox*>::iterator scene_it;

	processFrame();

	std::vector<gwc::PointEvent> point_events = consumePointEvents();

	
	for(std::vector<gwc::PointEvent>::iterator event_it = point_events.begin(); event_it != point_events.end(); event_it++) {
		if(event_it->status == gwc::TOUCHADDED) {
			std::string hit_object = hitTest(event_it->position.getX(), event_it->position.getY());
			if(hit_object != "") {
				assignTouchPoint(hit_object, event_it->point_id);
				std::cout << "Assigned touchpoint to " << hit_object << std::endl;
			}
		}
	}

	std::vector<gwc::GestureEvent> gesture_events = consumeGestureEvents();
	for(std::vector<gwc::GestureEvent>::iterator event_it = gesture_events.begin(); event_it != gesture_events.end(); event_it++) {
		std::map<std::string,MediaBox*>::iterator temp = scene_map.find(event_it->target);
		if(temp != scene_map.end()) {
			temp->second->handleEvent(*event_it);
		}
	}


	for(scene_it = scene_list.begin(); scene_it < scene_list.end(); scene_it++) {
		(*scene_it)->update();
	}
}

//Perform a hit test for a received point
std::string collectionViewer::hitTest(float x, float y) {
		
		vector<MediaBox*>::reverse_iterator scene_it;
		for(scene_it = scene_list.rbegin(); scene_it < scene_list.rend(); scene_it++)
		{
			if(!(*scene_it)->children.empty()) {
				vector<MediaBox*>::iterator child_it;
				for(child_it = (*scene_it)->children.begin(); child_it < (*scene_it)->children.end(); child_it++) {
					if(test_point(x, y, (*child_it)->x, (*child_it)->y, (*child_it)->width, (*child_it)->height, (*child_it)->rotation, (*child_it)->scale)) {
						return (*child_it)->object_id;
					}
				}
			}
			if(test_point(x, y, (*scene_it)->x, (*scene_it)->y, (*scene_it)->width, (*scene_it)->height, (*scene_it)->rotation, (*scene_it)->scale)) {
				return (*scene_it)->object_id;
			}
		}
		return "";
}


bool test_point(float point_x, float point_y, float box_x, float box_y, float box_width, float box_height, float box_angle, float box_scale) {
	
	float local_x = rotateAboutCenterX(point_x, point_y, box_x, box_y, -box_angle);
	float local_y = rotateAboutCenterY(point_x, point_y, box_x, box_y, -box_angle);
	
	if( (abs(local_x - box_x) <= box_width*box_scale/2) && (abs(local_y - box_y) <= box_height*box_scale/2) ) {
		return true;
	}
	return false;
}

float radsToDegrees(float rad) {
	return rad * 180 / 3.14159265358979323846264338;
}

float degreesToRads(float deg) {
	return 3.14159265358979323846264338 * deg / 180;
}

//Utility functions for doing coordinate transforms

float rotateAboutCenterX(float point_x, float point_y, float center_x, float center_y, float ref_angle) {

	float local_x = point_x - center_x;
	float local_y = point_y - center_y;
		
	float length = sqrt(local_x*local_x + local_y*local_y);
		
	return (length * cos(ref_angle + atan2(local_y, local_x)) + center_x);
}
	
float rotateAboutCenterY(float point_x, float point_y, float center_x, float center_y, float ref_angle) {

	float local_x = point_x - center_x;
	float local_y = point_y - center_y;
		
	float length = sqrt(local_x*local_x + local_y*local_y);
		
	return point_y = length * sin(ref_angle + atan2(local_y, local_x)) + center_y;
}
	
	
float getDrawingCoordX(float width, float height, float box_x, float box_y, float box_angle, float box_scale) {
	float length = sqrt(box_scale*box_scale*(width*width + height*height)/4);
	return (length * cos(box_angle + atan2(-height, -width)) + box_x);
}
	
float getDrawingCoordY(float width, float height, float box_x, float box_y, float box_angle, float box_scale) {
	float length = sqrt(box_scale*box_scale*(width*width + height*height)/4);
	return (length * sin(box_angle + atan2(-height, -width)) + box_y);
}