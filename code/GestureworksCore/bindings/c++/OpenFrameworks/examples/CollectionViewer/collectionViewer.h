////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    collectionViewer.h
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COLLECTIONVIEWER_20120515
#define COLLECTIONVIEWER_20120515
#include "ofMain.h"
#include "GestureWorksCore.h"
#include <math.h>

class MediaBox;
class PictureBox;
class MediaButton;
class MovieBox;

class collectionViewer : public ofBaseApp{

	public:

		vector<MediaBox*> scene_list;
		map<std::string, MediaBox*> scene_map;
		ofImage background;
		float window_position_x;
		float window_position_y;
		DWORD last_tick_count;

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);	

		std::string hitTest(float x, float y);
		float localToGlobalX(float local);
		float globalToLocalX(float global);
		float localToGlobalY(float local);
		float globalToLocalY(float global);

};

float radsToDegrees(float rad);
float degreesToRads(float deg);

float rotateAboutCenterX(float point_x, float point_y, float center_x, float center_y, float ref_angle);
float rotateAboutCenterY(float point_x, float point_y, float center_x, float center_y, float ref_angle);
float getDrawingCoordX(float width, float height, float box_x, float box_y, float box_angle, float box_scale);
float getDrawingCoordY(float width, float height, float box_x, float box_y, float box_angle, float box_scale);
bool test_point(float point_x, float point_y, float box_x, float box_y, float box_width, float box_height, float box_angle, float box_scale);

#endif