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
#include <iostream>

class Transform: public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::TRANSFORM)

	Transform() :
			_pos(), _dir(), _rot() {
	}

	Transform(Vector2D pos, Vector2D dir, float r, float s) :
			_pos(pos), _dir(dir), _rot(r) {
		setSpeed(s);
	}

	virtual ~Transform() {
	}

	Vector2D& getPos() {
		return _pos;
	}
	Vector2D& getDir() {
		return _dir;
	}
	
	const Vector2D& getPos() const {
		return _pos;
	}
	const Vector2D& getDir() const {
		return _dir;
	}


	void add_directional_speed(Vector2D extra_speed) {
		_dir = _dir + extra_speed;
	}

	void setPos(Vector2D& p) {
		_pos=p;
	}
	void setDir(Vector2D& d) {
		_dir = d;
	}
	void setPos(Vector2D&& p) {
		_pos = p;
	}
	void setDir(Vector2D&& d) {
		_dir = d;
	}


	float getRot() {
		return _rot;
	}

	void setRot(float r) {
		_rot = r;
	}
	
	float getSpeed() {
		return _dir.magnitude();
	}

	void setSpeed(float s) {
		_dir = _dir.normalize() * s;
	}

	void update(uint32_t delta_time) override {
		(void)delta_time;
		//Movement
		//std::cout << _pos << " + " << _dir << std::endl;
		_pos = _pos + _dir;
		//std::cout << "=>" << _pos << std::endl;
	}

private:
	Vector2D _pos;
	Vector2D _dir;
	float _rot;
};

