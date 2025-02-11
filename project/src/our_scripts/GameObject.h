// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once


#include <SDL.h>

#include "../utils/Vector2D.h"

class GameObject {
public:
	GameObject() :
			_pos(), _dir(), _width(), _height(), _rot(0.0f), _speed() {
	}
	virtual ~GameObject() {
	}

	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	inline float getHeight() const {
		return _height;
	}

	inline void setHeight(float height) {
		_height = height;
	}

	inline Vector2D& getPos() {
		return _pos;
	}

	inline Vector2D& getDir() {
		return _dir;
	}

	inline float& getSpeed() {
		return _speed;
	}

	inline float getWidth() const {
		return _width;
	}

	inline void setWidth(float width) {
		_width = width;
	}
	inline void setRotation(float rotation) {
		_rot = rotation;
	}
	inline float getRotation() const {
		return _rot;
	}

protected:

	Vector2D _pos;
	Vector2D _dir;
	float _speed;
	float _width;
	float _height;
	float _rot;
};

