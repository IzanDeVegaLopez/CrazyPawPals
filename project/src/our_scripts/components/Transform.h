// This file is part of the course TPV2@UCM - Samir Genaim


/*
* ELEMENTS:
* -Position
* -Direction
* -Width
* -Height
* -Rotation
* -Speed (float)
*/
#pragma once
#include "../../ecs/Component.h"
#include <cassert>

class Transform: public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::TRANSFORM)

	Transform() :
			_pos(), _dir(), _width(), _height(), _rot(), _speed() {
	}

	Transform(Vector2D pos, Vector2D dir, float w, float h, float r, float s, Vector2D prevDir = { 0,0 }) :
			_pos(pos), _dir(dir), _prevDir(prevDir), _width(w), _height(h), _rot(r), _speed(s) {
	}

	virtual ~Transform() {
	}

	void init(Vector2D pos, Vector2D dir, float w, float h, float r, float s, Vector2D prevDir = { 0,0 }) {
		_pos = pos;
		_dir = dir;
		_prevDir = prevDir;
		_width = w;
		_height = h;
		_rot = r;
		_speed = s;
	}

	Vector2D& getPos() {
		return _pos;
	}
	Vector2D& getDir() {
		return _dir;
	}
	Vector2D& getPrevDir()
	{
		return _prevDir;
	}
	void setPos(Vector2D& p) {
		_pos=p;
	}
	void setDir(Vector2D& d) {
		_dir = d;
	}
	//I wouldn�t let this one as public.. but opinions?
	void SetPrevDir(Vector2D& pD) {
		_prevDir = pD;
	} 

	float getWidth() {
		return _width;
	}

	void setWidth(float w) {
		_width = w;
	}

	float getHeight() {
		return _height;
	}

	void setHeight(float h) {
		_height = h;
	}

	float getRot() {
		return _rot;
	}

	void setRot(float r) {
		_rot = r;
	}

	float getSpeed() {
		return _speed;
	}

	void setSpeed(float s) {
		_speed = s;
	}

	void update(uint32_t delta_time) override {
		//Movement
		_pos = _pos + _dir * _speed;
		SetPrevDir(_dir);
	}

private:
	Vector2D _pos;
	Vector2D _dir;
	Vector2D _prevDir;
	float _width;
	float _height;
	float _rot;
	float _speed;
};

