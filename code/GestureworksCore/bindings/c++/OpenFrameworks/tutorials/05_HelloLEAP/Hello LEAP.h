////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    Hello LEAP.h
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#include "ofMain.h"
#include "GestureWorksCore.h"
#include "CameraListener.h"
#include <math.h>


DWORD LEAP_thread(LPVOID lpdwThreadParam);

class helloLEAP : public ofBaseApp{

	public:

		DWORD last_tick_count;

		void setup();
		void update();
		void draw();	
};

