////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    MediaBoxes.cpp
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#include "MediaBoxes.h"


void MediaButton::draw() {
	float draw_x = getDrawingCoordX(width, height, x, y, rotation, scale);
	float draw_y = getDrawingCoordY(width, height, x, y, rotation, scale);
	ofTranslate(draw_x, draw_y);
	ofRotate(radsToDegrees(rotation));
	button_image.draw(0, 0, width * scale, height * scale);
	ofRotate(radsToDegrees(-rotation));
	ofTranslate(-draw_x, -draw_y);
}

void MediaButton::controlTagged(std::string control_id) {}

void MediaButton::setX(float new_x) {
	x = new_x;
}

void MediaButton::setY(float new_y) {
	y = new_y;
}

void MediaButton::setScale(float new_scale) {
	scale = new_scale;
}

void MediaButton::setRotation(float new_angle) {
	rotation = new_angle;
}

void MediaButton::loadImage(std::string filename) {
	button_image.loadImage(filename);
}

void MediaButton::handleEvent(gwc::GestureEvent evt) {
	if(evt.target == object_id) {
		if(evt.gesture_id == "n-tap") {
			parent->controlTagged(object_id);
		}
		//Move parent to front
		vector<MediaBox*>::iterator scene_it;
		for( scene_it = sketch->scene_list.begin(); scene_it < sketch->scene_list.end(); scene_it++) {
			if( (*scene_it)->object_id == parent->object_id ){
				scene_it = sketch->scene_list.erase(scene_it);
				sketch->scene_list.push_back(parent);
				return;
			}
		}
	}
}

void MediaButton::update() {}


void PictureBox::controlTagged(std::string control_id) {
	if(control_id == object_id + ".toggle") {
		if(info_mode) { info_toggle.loadImage("GW_info.png"); } else { info_toggle.loadImage("GW_return.png"); }
		info_mode = !info_mode;
	} else if(control_id == object_id + ".close") {
		//If close was tapped, remove this object from the scene
		vector<MediaBox*>::iterator scene_it;
		for( scene_it = sketch->scene_list.begin(); scene_it < sketch->scene_list.end(); scene_it++) {
			if( (*scene_it)->object_id == object_id ){
				scene_it = sketch->scene_list.erase(scene_it);
				return;
			}
		}
	}
}

void PictureBox::setX(float new_x) {
	info_toggle.setX(info_toggle.x + (new_x - x));
	close.setX(close.x + (new_x - x));
	x = new_x;
}

void PictureBox::setY(float new_y) {
	info_toggle.setY(info_toggle.y + (new_y - y));
	close.setY(close.y + (new_y - y));
	y = new_y;
}

void PictureBox::setScale(float new_scale) {
	scale = new_scale;
	float temp_x;
	float new_info_toggle_x = x + (scale*width/2) - 95;
	float new_info_toggle_y = y + (scale*height/2) + 35;
	float new_close_x = x + (scale*width/2) - 35;
	float new_close_y = y + (scale*height/2) + 35;
	temp_x = rotateAboutCenterX(new_info_toggle_x, new_info_toggle_y, x, y, rotation);
	new_info_toggle_y = rotateAboutCenterY(new_info_toggle_x, new_info_toggle_y, x, y, rotation);
	new_info_toggle_x = temp_x;
	temp_x = rotateAboutCenterX(new_close_x, new_close_y, x, y, rotation);
	new_close_y = rotateAboutCenterY(new_close_x, new_close_y, x, y, rotation);
	new_close_x = temp_x;
	info_toggle.setX(new_info_toggle_x);
	info_toggle.setY(new_info_toggle_y);
	info_toggle.setRotation(rotation);
	close.setX(new_close_x);
	close.setY(new_close_y);
	close.setRotation(rotation);
}

void PictureBox::setRotation(float new_angle) {
	rotation = new_angle;
	float temp_x;
	float new_info_toggle_x = x + (scale*width/2) - 95;
	float new_info_toggle_y = y + (scale*height/2) + 35;
	float new_close_x = x + (scale*width/2) - 35;
	float new_close_y = y + (scale*height/2) + 35;
	temp_x = rotateAboutCenterX(new_info_toggle_x, new_info_toggle_y, x, y, rotation);
	new_info_toggle_y = rotateAboutCenterY(new_info_toggle_x, new_info_toggle_y, x, y, rotation);
	new_info_toggle_x = temp_x;
	temp_x = rotateAboutCenterX(new_close_x, new_close_y, x, y, rotation);
	new_close_y = rotateAboutCenterY(new_close_x, new_close_y, x, y, rotation);
	new_close_x = temp_x;
	info_toggle.setX(new_info_toggle_x);
	info_toggle.setY(new_info_toggle_y);
	info_toggle.setRotation(rotation);
	close.setX(new_close_x);
	close.setY(new_close_y);
	close.setRotation(rotation);
}

void PictureBox::loadImage(std::string filename) {
	the_image.loadImage(filename);
	the_image.setImageType(OF_IMAGE_COLOR_ALPHA);
}

void PictureBox::loadToggle(std::string filename) {
	toggle_image.loadImage(filename);
	toggle_image.setImageType(OF_IMAGE_COLOR);
}


void PictureBox::draw() {
	float draw_x = getDrawingCoordX(width, height, x, y, rotation, scale);
	float draw_y = getDrawingCoordY(width, height, x, y, rotation, scale);
	ofTranslate(draw_x, draw_y);
	ofRotate(radsToDegrees(rotation));
	if(!info_mode) {
		the_image.draw(0, 0, width * scale, height * scale);
	} else {
		toggle_image.draw(0, 0, width * scale, height * scale);
	}
	ofRotate(radsToDegrees(-rotation));
	ofTranslate(-draw_x, -draw_y);
	
	info_toggle.draw();
	close.draw();
}

void PictureBox::handleEvent(gwc::GestureEvent evt) {
	if(evt.target == object_id) {
		if(evt.gesture_id == "n-drag") {
			setY(y + evt.dimension_values.find("drag_dy")->second);
			setX(x + evt.dimension_values.find("drag_dx")->second);
		} 
		else if(evt.gesture_id == "n-rotate") {

			float rotation_angle = degreesToRads(evt.dimension_values.find("rotate_dtheta")->second);

			float temp_x = rotateAboutCenterX(x, y, evt.x, evt.y, rotation_angle);
			float temp_y = rotateAboutCenterY(x, y, evt.x, evt.y, rotation_angle);
			setX(temp_x);
			setY(temp_y);
			
			setRotation(rotation + degreesToRads(evt.dimension_values.find("rotate_dtheta")->second));
		}
		else if(evt.gesture_id == "n-scale") {
			setScale(scale + evt.dimension_values.find("scale_dsx")->second);
		}
	}
}

void PictureBox::update() {}


void MovieBox::controlTagged(std::string control_id) {
	
	if(control_id == object_id + ".close") {
		//If close was tapped, remove this object from the scene
		vector<MediaBox*>::iterator scene_it;
		for( scene_it = sketch->scene_list.begin(); scene_it < sketch->scene_list.end(); scene_it++) {
			if( (*scene_it)->object_id == object_id ){
				scene_it = sketch->scene_list.erase(scene_it);
				return;
			}
		}
	}
	//Disable play/stop buttons during info mode
	if(info_mode) {
		if(control_id == object_id + ".toggle") {	
			info_toggle.loadImage("GW_info.png");
			info_mode = false;
		}
	} else {
		if(control_id == object_id + ".toggle") {
			info_toggle.loadImage("GW_return.png");
			info_mode = true;
			return;
		} else if( (control_id == object_id + ".play") && (movie_paused || (!movie_playing))) {
			the_movie.setPosition(movie_time);
			the_movie.play();
			movie_playing = true;
			movie_paused = false;
			play.loadImage("GW_pause.png");
			return;
		} else if(control_id == object_id + ".stop") {
			the_movie.stop();
			movie_time = 0;
			movie_paused = false;
			movie_playing = false;
			play.loadImage("GW_play.png");
			return;
		} else {
			if(movie_playing) {
				movie_time = the_movie.getPosition();
				movie_paused = true;
				the_movie.stop();

				play.loadImage("GW_play.png");
				return;
			}
		}
	}
}


void MovieBox::setX(float new_x) {
	play.setX(play.x + (new_x - x));
	stop.setX(stop.x + (new_x - x));
	info_toggle.setX(info_toggle.x + (new_x - x));
	close.setX(close.x + (new_x - x));
	x = new_x;
}

void MovieBox::setY(float new_y) {
	play.setY(play.y + (new_y - y));
	stop.setY(stop.y + (new_y - y));
	info_toggle.setY(info_toggle.y + (new_y - y));
	close.setY(close.y + (new_y - y));
	y = new_y;
}

void MovieBox::setScale(float new_scale) {
	scale = new_scale;
	float temp_x = 0;
	float temp_y = 0;
	float new_play_x = x - scale*width/2 + 35;
	float new_play_y = y + scale*height/2 + 35;
	float new_stop_x = x - scale*width/2 + 95;
	float new_stop_y = y + scale*height/2 + 35;
	float new_toggle_x = x + (scale*width/2) - 95;
	float new_toggle_y = y + (scale*height/2) + 35;
	float new_close_x = x + (scale*width/2) - 35;
	float new_close_y = y + (scale*height/2) + 35;
	temp_x = rotateAboutCenterX(new_play_x, new_play_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_play_x, new_play_y, x, y, rotation);
	new_play_x = temp_x;
	new_play_y = temp_y;
	temp_x = rotateAboutCenterX(new_stop_x, new_stop_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_stop_x, new_stop_y, x, y, rotation);
	new_stop_x = temp_x;
	new_stop_y = temp_y;
	temp_x = rotateAboutCenterX(new_toggle_x, new_toggle_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_toggle_x, new_toggle_y, x, y, rotation);
	new_toggle_x = temp_x;
	new_toggle_y = temp_y;
	temp_x = rotateAboutCenterX(new_close_x, new_close_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_close_x, new_close_y, x, y, rotation);
	new_close_x = temp_x;
	new_close_y = temp_y;

	play.setX(new_play_x);
	play.setY(new_play_y);
	stop.setX(new_stop_x);
	stop.setY(new_stop_y);
	info_toggle.setX(new_toggle_x);
	info_toggle.setY(new_toggle_y);
	close.setX(new_close_x);
	close.setY(new_close_y);
}

void MovieBox::setRotation(float new_angle) {
	rotation = new_angle;
		
	float temp_x = 0;
	float temp_y = 0;
	float new_play_x = x - scale*width/2 + 35;
	float new_play_y = y + scale*height/2 + 35;
	float new_stop_x = x - scale*width/2 + 95;
	float new_stop_y = y + scale*height/2 + 35;
	float new_toggle_x = x + (scale*width/2) - 95;
	float new_toggle_y = y + (scale*height/2) + 35;
	float new_close_x = x + (scale*width/2) - 35;
	float new_close_y = y + (scale*height/2) + 35;
	temp_x = rotateAboutCenterX(new_play_x, new_play_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_play_x, new_play_y, x, y, rotation);
	new_play_x = temp_x;
	new_play_y = temp_y;
	temp_x = rotateAboutCenterX(new_stop_x, new_stop_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_stop_x, new_stop_y, x, y, rotation);
	new_stop_x = temp_x;
	new_stop_y = temp_y;
	temp_x = rotateAboutCenterX(new_toggle_x, new_toggle_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_toggle_x, new_toggle_y, x, y, rotation);
	new_toggle_x = temp_x;
	new_toggle_y = temp_y;
	temp_x = rotateAboutCenterX(new_close_x, new_close_y, x, y, rotation);
	temp_y = rotateAboutCenterY(new_close_x, new_close_y, x, y, rotation);
	new_close_x = temp_x;
	new_close_y = temp_y;
	play.setX(new_play_x);
	play.setY(new_play_y);
	play.setRotation(new_angle);
	stop.setX(new_stop_x);
	stop.setY(new_stop_y);
	stop.setRotation(new_angle);
	info_toggle.setX(new_toggle_x);
	info_toggle.setY(new_toggle_y);
	info_toggle.setRotation(new_angle);
	close.setX(new_close_x);
	close.setY(new_close_y);
	close.setRotation(new_angle);
}

void MovieBox::loadMovie(std::string filename) {
	the_movie.loadMovie(filename);
}

void MovieBox::loadToggle(std::string filename) {
	toggle_image.loadImage(filename);
	toggle_image.setImageType(OF_IMAGE_COLOR);
}

void MovieBox::draw() {
	float draw_x = getDrawingCoordX(width, height, x, y, rotation, scale);
	float draw_y = getDrawingCoordY(width, height, x, y, rotation, scale);
	ofTranslate(draw_x, draw_y);
	ofRotate(radsToDegrees(rotation));
	if(!info_mode) {
		the_movie.draw(0, 0, width * scale, height * scale);
	} else {
		toggle_image.draw(0, 0, width * scale, height * scale);
	}
	ofRotate(radsToDegrees(-rotation));
	ofTranslate(-draw_x, -draw_y);
	
	play.draw();
	stop.draw();
	info_toggle.draw();
	close.draw();
}

void MovieBox::handleEvent(gwc::GestureEvent evt) {
	if(evt.target == object_id) {
		if(evt.gesture_id == "n-drag") {
			setY(y + evt.dimension_values.find("drag_dy")->second);
			setX(x + evt.dimension_values.find("drag_dx")->second);
		} 
		else if(evt.gesture_id == "n-rotate") {
			setRotation(rotation + degreesToRads(evt.dimension_values.find("rotate_dtheta")->second));
		}
		else if(evt.gesture_id == "n-scale") {
			setScale(scale + evt.dimension_values.find("scale_dsx")->second);
		}
	}
}

//This will reset the play button and condition variable when the movie finishes
void MovieBox::update() { 
	the_movie.idleMovie();
	if(the_movie.getIsMovieDone()) {
		movie_playing = false;
		play.loadImage("GW_play.png");
	}
}