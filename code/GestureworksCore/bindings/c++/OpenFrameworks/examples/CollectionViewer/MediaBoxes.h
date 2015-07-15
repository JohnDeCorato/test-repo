////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  GestureWorks
//
//  File:    MediaBoxes.h
//  Authors:  Ideum
//             
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MEDIABOX_20120515
#define MEDIABOX_20120515

#include <vector>
#include <math.h>
#include "collectionViewer.h"

class MediaBox {
	
public:
	float x;
	float y;
	float rotation;
	float scale;
	float width;
	float height;
	collectionViewer* sketch;
	std::string object_id;
	std::vector<MediaBox*> scene;
	std::vector<MediaBox*> children;
	MediaBox* parent;

	virtual ~MediaBox() {}
	MediaBox( collectionViewer* sketch, std::vector<MediaBox*> scene, std::string object_id, float width, float height)
		: object_id(object_id), sketch(sketch), width(width), height(height), scene(scene) {
		children = std::vector<MediaBox*>();
		parent = 0;
		x = 0;
		y = 0;
		rotation = 0;
		scale = 1;
	}
	
	
	virtual void handleEvent(gwc::GestureEvent evt) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void setX(float new_x) = 0;
	virtual void setY(float new_y) = 0;
	virtual void setScale(float new_scale) = 0;
	virtual void setRotation(float new_angle) = 0;
	virtual void controlTagged(std::string control_id) = 0;

	void setParent(MediaBox* parent) {
		this->parent = parent;
	}	
};

class MediaButton : public MediaBox {

public:
	ofImage button_image;

	MediaButton(collectionViewer* sketch, std::vector<MediaBox*> scene, std::string object_id, float width, float height, MediaBox* parent) : MediaBox(sketch, scene, object_id, width, height) {
		setParent(parent);

		x = 0;
		y = 0;
		rotation = 0;
		setScale(1);
	}

	void draw();
	void setX(float new_x);
	void setY(float new_y);
	void setScale(float new_scale);
	void setRotation(float new_angle);
	void controlTagged(std::string control_id);

	void loadImage(std::string filename);
	void update();
	void handleEvent(gwc::GestureEvent evt);
};

class PictureBox : public MediaBox {


public:
	ofImage the_image;
	ofImage toggle_image;
	MediaButton info_toggle;
	MediaButton close;
	bool info_mode;

	PictureBox(collectionViewer* sketch, std::vector<MediaBox*> scene, std::string object_id, float width, float height) : MediaBox(sketch, scene, object_id, width, height),
		info_toggle(MediaButton(sketch, scene, object_id + ".toggle", 50, 50, this)),
		close(MediaButton(sketch, scene, object_id + ".close", 50, 50, this)) {
		the_image = ofImage();
		toggle_image = ofImage();

		info_mode = false;

		x = 0;
		y = 0;
		rotation = 0;
		info_toggle.loadImage("GW_info.png");
		close.loadImage("GW_close.png");
		children.push_back( (MediaBox*) &info_toggle);
		children.push_back( (MediaBox*) &close);

		setScale(1);
	}

	void draw();
	void setX(float new_x);
	void setY(float new_y);
	void setScale(float new_scale);
	void setRotation(float new_angle);
	void controlTagged(std::string control_id);

	void loadImage(std::string filename);
	void loadToggle(std::string filename);
	void update();
	void handleEvent(gwc::GestureEvent evt);
};

class MovieBox : public MediaBox {

public :
	ofVideoPlayer the_movie;
	ofImage toggle_image;
	MediaButton play;
	MediaButton stop;
	MediaButton info_toggle;
	MediaButton close;
	bool movie_playing;
	bool movie_paused;
	float movie_time;
	bool info_mode;

	MovieBox(collectionViewer* sketch, std::vector<MediaBox*> scene, std::string object_id, float width, float height) : MediaBox(sketch, scene, object_id, width, height),
		play(MediaButton(sketch, scene, object_id +".play", 50, 50, this)),
		stop(MediaButton(sketch, scene, object_id + ".stop", 50, 50, this)),
		info_toggle(MediaButton(sketch, scene, object_id + ".toggle", 50, 50, this)),
		close(MediaButton(sketch, scene, object_id + ".close", 50, 50, this)) {
		the_movie = ofVideoPlayer();
		movie_time = 0;
		movie_playing = false;
		movie_paused = false;
		info_mode = false;

		x = 0;
		y = 0;
		rotation = 0;
		play.loadImage("GW_play.png");
		stop.loadImage("GW_stop.png");
		info_toggle.loadImage("GW_info.png");
		close.loadImage("GW_close.png");
		children.push_back( (MediaBox*) &play);
		children.push_back( (MediaBox*) &stop);
		children.push_back( (MediaBox*) &info_toggle);
		children.push_back( (MediaBox*) &close);

		setScale(1);
	}
	void draw();
	void update();
	void setX(float new_x);
	void setY(float new_y);
	void setScale(float new_scale);
	void setRotation(float new_angle);
	void controlTagged(std::string control_id);

	void loadMovie(std::string filename);
	void loadToggle(std::string filename);
	void handleEvent(gwc::GestureEvent evt);
};


#endif //MEDIABOX_20120515
