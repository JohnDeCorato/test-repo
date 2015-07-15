////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    main.cpp
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#include "ofMain.h"
#include "Hello TUIO.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
    
	
	ofAppGlutWindow window;

	//Tell OpenFrameworks to fullscreen the app at 1920 by 1080 pixels
	ofSetupOpenGL(&window, 1920,1080, OF_FULLSCREEN);

	//For attaching Gestureworks by window name, we need to set it explicitly
	ofSetWindowTitle("Hello Multitouch!");

	//And go!
	ofRunApp( new helloTUIO());
}

