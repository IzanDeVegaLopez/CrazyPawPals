// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../game/GameObject.h"
#include "NinjaState.h"

class Texture;
class NinjaWS: public GameObject {
public:
	enum State {
		STANDING, JUMPING, WALKING, DUCKING, ATTACKING
	};
	NinjaWS();
	virtual ~NinjaWS();
	void update() override;
	void render() override;
	void handleInput() override;

	inline void setImg(Texture *img) {
		_img = img;
	}

	inline void setState(State s) {
		switch (s) {
		case STANDING:
			_state = _standig_state;
			break;
		case JUMPING:
			_state = _jumping_state;
			break;
		case WALKING:
			_state = _walking_state;
			break;
		case DUCKING:
			_state = _ducking_state;
			break;
		case ATTACKING:
			_state = _attacking_state;
			break;
		default:
			assert(false);
			break;
		}
		_state->enter(*this);
	}
private:
	Texture *_img;


	NinjaState *_state;
	NinjaState *_standig_state;
	NinjaState *_jumping_state;
	NinjaState *_walking_state;
	NinjaState *_ducking_state;
	NinjaState *_attacking_state;

};

