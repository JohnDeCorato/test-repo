////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:    CameraListener.cpp
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#include "CameraListener.h"

#define LEAP_TIMER 100
#define LEAP_ZMAX 200
#define LEAP_ZMIN -20

#define LEAP_LEFT -200
#define LEAP_RIGHT 200
#define LEAP_UP 300
#define LEAP_DOWN 50

static std::deque<gwc::touchpoint> leap_queue;
static std::vector<long> leap_ids;


void CameraListener::onInit(const Leap::Controller& controller){}

void CameraListener::onConnect(const Leap::Controller& controller){}

void CameraListener::onDisconnect(const Leap::Controller& controller){}

void CameraListener::onFrame(const Leap::Controller& controller){
	
	const Leap::Frame frame = controller.frame();
	const Leap::HandList hands = frame.hands();
	std::vector<int32_t> processed_ids;
	std::vector<Leap::Finger> all_fingers;
	if(!hands.empty()){
		const Leap::Hand& hand = hands[0];
		for(int h = 0; h < hands.count(); h++){
			const Leap::FingerList fingers = hand.fingers();
			for(int f = 0; f < fingers.count(); f++){
				all_fingers.push_back(fingers[f]);
			}
		}
		for(int i = 0; i < all_fingers.size(); i++){
			
				gwc::touchpoint tp;
				double x, y, z;
				Leap::Vector pos;
				pos = all_fingers[i].tipPosition();
				x = pos[0];
				y = pos[1];
				z = pos[2];

				double adj_x = (x - LEAP_LEFT) / abs(LEAP_RIGHT - LEAP_LEFT);
				double adj_y = (LEAP_UP + LEAP_DOWN - y) / abs(LEAP_DOWN - LEAP_UP);
				
				long id = (long)all_fingers[i].id();
				tp.init(id, adj_x, adj_y, z, 1, 1);
		
				if( (z < LEAP_ZMIN) || (z > LEAP_ZMAX) ){
					continue;
				}
				
				if(std::find(leap_ids.begin(), leap_ids.end(), id) == leap_ids.end()){
					leap_ids.push_back(id);
					tp.status = gwc::TOUCHADDED;
				}
				else{
					tp.status = gwc::TOUCHUPDATE;
				}
				addEvent( tp );
				processed_ids.push_back(id);
			}

			for(int i = 0; i < leap_ids.size(); i++){
				if(std::find(processed_ids.begin(), processed_ids.end(), leap_ids[i]) == processed_ids.end()){
					gwc::touchpoint tp;
					tp.init(leap_ids[i], 0, 0, 0, 0, 0);
					tp.status = gwc::TOUCHREMOVED;
					
					addEvent( tp );
					leap_ids.erase(leap_ids.begin() + i);
				}
			}
		}
	
	else{
		for(int i = 0; i < leap_ids.size(); i++){
			gwc::touchpoint tp;
			tp.init(leap_ids[i], 0, 0, 0, 0, 0);
			tp.status = gwc::TOUCHREMOVED;
			addEvent( tp );
		}
		leap_ids.clear();
	}
}