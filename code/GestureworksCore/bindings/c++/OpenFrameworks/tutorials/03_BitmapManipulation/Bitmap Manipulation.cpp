////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    Bitmap Manipulation.cpp
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ofMain.h"
#include "Bitmap Manipulation.h"




TouchObject::TouchObject() {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	rotation = 0;
	scale = 0;
}

TouchObject::TouchObject(float x, float y, float width, float height, float rotation, float scale) 
	: x(x), y(y), width(width), height(height), rotation(rotation), scale(scale) {}


//--------------------------------------------------------------
void bitmapManipulation::setup(){
	
	ofEnableAlphaBlending();
	ofSetFullscreen(true);

	last_tick_count = GetTickCount();

	logo.loadImage("gw_logo.png");

	logo1_dimensions = TouchObject(1360, 440, 200, 200, 0, 1);
	logo2_dimensions = TouchObject(560, 440, 200, 200, 0, 1);
		
	if(loadGestureWorks("GestureWorksCore.dll")) { 
		std::cout << "Error loading gestureworks dll" << std::endl; 
		exit(); 
	}
	if(!loadGML("basic_manipulation.gml")) { 
		std::cout << "Could not find gml file" << std::endl; 
		exit(); 
	}

	initializeGestureWorks(1920,1080);

	if( !registerWindowForTouchByName("Bitmap Manipulation") ) { 
		std::cout << "Could not register target window for touch." << std::endl; 
		exit(); 
	}

	registerTouchObject("logo1");
	registerTouchObject("logo2");

	addGesture("logo1","n-drag");
	addGesture("logo1","n-rotate");
	addGesture("logo1","n-scale");

	addGesture("logo2","n-drag");
	addGesture("logo2","n-rotate");
	addGesture("logo2","n-scale");

}

//This function is the last to get called every cycle, and is responsible
//for generating all screen artifacts
void bitmapManipulation::draw(){

	ofBackground(ofColor(169,169,169));

	float height1 = logo1_dimensions.height;
	float width1 = logo1_dimensions.width;
	float rotation1 = logo1_dimensions.rotation;
	float x1 = logo1_dimensions.x;
	float y1 = logo1_dimensions.y;
	float scale1 = logo1_dimensions.scale;
	float draw_x1 = getDrawingCoordX(width1, height1, x1, y1, rotation1, scale1);
	float draw_y1 = getDrawingCoordY(width1, height1, x1, y1, rotation1, scale1);




	float height2 = logo2_dimensions.height;
	float width2 = logo2_dimensions.width;
	float rotation2 = logo2_dimensions.rotation;
	float x2 = logo2_dimensions.x;
	float y2 = logo2_dimensions.y;
	float scale2 = logo2_dimensions.scale;
	float draw_x2 = getDrawingCoordX(width2, height2, x2, y2, rotation2, scale2);
	float draw_y2 = getDrawingCoordY(width2, height2, x2, y2, rotation2, scale2);

	ofTranslate(draw_x2, draw_y2);
	ofRotate(radsToDegrees(rotation2));
	logo.draw(0, 0, width2 * scale2, height2 * scale2);
	ofRotate(radsToDegrees(-rotation2));
	ofTranslate(-draw_x2, -draw_y2);

	ofTranslate(draw_x1, draw_y1);
	ofRotate(radsToDegrees(rotation1));
	logo.draw(0, 0, width1 * scale1, height1 * scale1);
	ofRotate(radsToDegrees(-rotation1));
	ofTranslate(-draw_x1, -draw_y1);

}

//The update function gets called once per cycle prior to the draw function
void bitmapManipulation::update(){

	//Updating once every 16 milliseconds corresponds to a rate of approximately 62.5 frames per second
	if(GetTickCount() - last_tick_count < 16) {
		return;
	} else { last_tick_count = GetTickCount(); }
  
	processFrame();

	std::vector<gwc::PointEvent> point_events = consumePointEvents();

	//Hit-test new points to see if they struck one of the logo objects
	for(std::vector<gwc::PointEvent>::iterator event_it = point_events.begin(); event_it != point_events.end(); event_it++) {
		if(event_it->status == gwc::TOUCHADDED)
		{
			//All new touchpoints must go through hit testing to see if they apply to our bitmap manipulation; since logo1 is always on top, we check it first
			if( test_point(	event_it->position.getX(),
							event_it->position.getY(),
							logo1_dimensions.x,
							logo1_dimensions.y,
							logo1_dimensions.width,
							logo1_dimensions.height,
							logo1_dimensions.rotation, 
							logo1_dimensions.scale)
							)
			{
				assignTouchPoint("logo1",event_it->point_id);
			}
			else if( test_point(	event_it->position.getX(),
									event_it->position.getY(),
									logo2_dimensions.x,
									logo2_dimensions.y,
									logo2_dimensions.width,
									logo2_dimensions.height,
									logo2_dimensions.rotation,
									logo2_dimensions.scale)
									)
			{
				assignTouchPoint("logo2",event_it->point_id);
			}
		}
	}

	//Interpret gesture events
	
	std::vector<gwc::GestureEvent> gesture_events = consumeGestureEvents();

	for(std::vector<gwc::GestureEvent>::iterator gesture_it = gesture_events.begin(); gesture_it != gesture_events.end(); gesture_it++) {
		if(gesture_it->target == "logo1") {
			if(gesture_it->gesture_id == "n-drag") {
				float new_x = logo1_dimensions.x + gesture_it->values["drag_dx"];
				float new_y = logo1_dimensions.y + gesture_it->values["drag_dy"];
				logo1_dimensions.x = new_x;
				logo1_dimensions.y = new_y;
			}
			else if(gesture_it->gesture_id == "n-rotate") {
				//Rotation is about a specific point, so we need to do a coordinate transform and adjust
				//not only the object's rotation, but it's x and y values as well
				float temp_x;
				float temp_y;
				float rotation_angle = degreesToRads(gesture_it->values["rotate_dtheta"]);
					
					//If we have points down, move the box; if there are no points, this is from gesture inertia and there is no
					//center about which to rotate
					if(gesture_it->n != 0) {
						float temp_x = rotateAboutCenterX( logo1_dimensions.x, logo1_dimensions.y, gesture_it->x, gesture_it->y, rotation_angle);
						float temp_y = rotateAboutCenterY( logo1_dimensions.x, logo1_dimensions.y, gesture_it->x, gesture_it->y, rotation_angle);
					}

				logo1_dimensions.rotation = logo1_dimensions.rotation + rotation_angle;
			}
			else if(gesture_it->gesture_id == "n-scale") {
				logo1_dimensions.scale = logo1_dimensions.scale + gesture_it->values["scale_dsx"];
			}

		}
		else if(gesture_it->target == "logo2") {
			if(gesture_it->gesture_id == "n-drag") {
				float new_x = logo2_dimensions.x + gesture_it->values["drag_dx"];
				float new_y = logo2_dimensions.y + gesture_it->values["drag_dy"];
				logo2_dimensions.x = new_x;
				logo2_dimensions.y = new_y;
			}
			else if(gesture_it->gesture_id == "n-rotate") {
				//Rotation is about a specific point, so we need to do a coordinate transform and adjust
				//not only the object's rotation, but it's x and y values as well
				float rotation_angle = degreesToRads(gesture_it->values["rotate_dtheta"]);
				float temp_x = rotateAboutCenterX( logo2_dimensions.x, logo2_dimensions.y, gesture_it->x, gesture_it->y, rotation_angle);
				float temp_y = rotateAboutCenterY( logo2_dimensions.x, logo2_dimensions.y, gesture_it->x, gesture_it->y, rotation_angle);

				logo2_dimensions.rotation = logo2_dimensions.rotation + rotation_angle;
			}
			else if(gesture_it->gesture_id == "n-scale") {
				logo2_dimensions.scale = logo2_dimensions.scale + gesture_it->values["scale_dsx"];
			}
		}
	}
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