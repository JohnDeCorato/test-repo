////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    Bitmap Manipulation.h
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#include "ofMain.h"
#include "GestureWorksCore.h"
#include <math.h>


class TouchObject {

	public:

		float x;
		float y;
		float width;
		float height;
		float rotation;
		float scale;

		TouchObject();
		TouchObject(float x, float y, float width, float height, float rotation, float scale);
};

class bitmapManipulation : public ofBaseApp{

	public:

		TouchObject logo1_dimensions;
		TouchObject logo2_dimensions;

		ofImage logo;

		DWORD last_tick_count;

		void setup();
		void update();
		void draw();

};



float radsToDegrees(float rad);
float degreesToRads(float deg);

//Hit test function
bool test_point(float point_x, float point_y, float box_x, float box_y, float box_width, float box_height, float box_angle, float box_scale);

//Utility functions for doing coordinate transforms
float rotateAboutCenterX(float point_x, float point_y, float center_x, float center_y, float ref_angle);
float rotateAboutCenterY(float point_x, float point_y, float center_x, float center_y, float ref_angle);
float getDrawingCoordX(float width, float height, float box_x, float box_y, float box_angle, float box_scale);
float getDrawingCoordY(float width, float height, float box_x, float box_y, float box_angle, float box_scale);

