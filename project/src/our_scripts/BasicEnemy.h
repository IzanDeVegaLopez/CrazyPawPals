// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../game/GameObject.h"
#include "BasicEnemyState.h"
#include "WalkingState.h"
#include "AttackingState.h"
#include "DyingState.h"
#include "InactiveState.h"

class Texture;
class BasicEnemy : public GameObject {
public:
	enum State {
		WALKING, ATTACKING, DYING, INACTIVE
	};
	BasicEnemy();
	virtual ~BasicEnemy();
	void update() { _state->update(*this); };
	void render() override;
	void handleInput() { _state->handleInput(*this); } ;

	inline void setImg(Texture *img) {
		_img = img;
	}

	inline void setState(State s) {
		switch (s) {
		case WALKING:
			_state = _walking_state;
			break;
		case ATTACKING:
			_state = _attacking_state;
			break;
		case DYING:
			_state = _dying_state;
			break;
		case INACTIVE:
			_state = _inactive_state;
			break;
		default:
			assert(false);
			break;
		}
		_state->enter(*this);
	}
private:
	Texture *_img;


	BasicEnemyState* _state;
	static WalkingState* _walking_state;
	static AttackingState* _attacking_state;
	static DyingState* _dying_state;
	static InactiveState* _inactive_state;

};

