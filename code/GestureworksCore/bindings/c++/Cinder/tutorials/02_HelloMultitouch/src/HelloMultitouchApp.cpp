////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    HelloMultitouchApp.cpp
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GestureWorksCore.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"
#include <math.h>

using namespace ci;
using namespace ci::app;

class HelloMultitouchApp : public AppBasic {
  public:
	Font				mFont;
	gl::TextureFontRef	mTextureFont;

	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();

};

void HelloMultitouchApp::prepareSettings( Settings *settings ) {
	settings->setWindowSize( 1920, 1080 );
	settings->setFullScreen(true);
	settings->setTitle( "Hello Multitouch!" );
	settings->setFrameRate( 60.0f );
}

std::map<int,gwc::Point> active_points;

void HelloMultitouchApp::setup() {
	// create font
	mFont = Font( "Arial", 22 );
	mTextureFont = gl::TextureFont::create( mFont );

	active_points = std::map<int,gwc::Point>();

	if(loadGestureWorks("C:\\GestureWorks\\GestureWorksCoreWin7.dll")) { 
		console() << "Error loading gestureworks dll" << std::endl; 
	}

	if(!loadGML("C:\\GestureWorks\\basic_manipulation.gml")) { 
		console() << "Could not find gml file" << std::endl; 
	}

	initializeGestureWorks(1920,1080);

	if( !registerWindowForTouchByName( "Hello Multitouch!" ) ) { 
		console() << "Could not register target window for touch." << std::endl;
	}
}

void HelloMultitouchApp::update() {
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

void HelloMultitouchApp::draw() {
	// clear out the window
	gl::clear( Color( 0.66f, 0.66f, 0.66f ), true );
	gl::enableAlphaBlending();
	gl::color( Color( 1.0f, 0.2f, 0.0f ) );

	//For each active point
	for(std::map<int,gwc::Point>::iterator points_it = active_points.begin(); points_it != active_points.end(); points_it++)
	{
		float x = points_it->second.getX();
		float y = points_it->second.getY();

		//Generate a circle at this point's location
		gl::drawSolidCircle( Vec2f( x, y ), 40.0f );
		gl::drawStrokedCircle( Vec2f( x, y ), 50.0f );

		//Generate a stringstream for each value with which we're concerned
		std::stringstream xvals; xvals << x;
		std::stringstream yvals; yvals << y;
		std::stringstream ids; ids << points_it->first;

		//Annotate the circle we just drew with the id, x and y values for the corresponding point
		mTextureFont->drawString("ID: " + ids.str() + "    X: " + xvals.str() + "  " +  " Y: " + yvals.str(),  Vec2f(x + 40.0f, y - 40.0f) );
	}
}

CINDER_APP_BASIC( HelloMultitouchApp, RendererGl(0) )
