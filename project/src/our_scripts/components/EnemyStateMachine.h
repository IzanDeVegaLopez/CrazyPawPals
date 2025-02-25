// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
//#include "../states/DyingState.h"
//#include "../states/InactiveState.h"
#include "../../ecs/Component.h"

class Texture;
class WalkingState;
class AttackingState;
class State;

class EnemyStateMachine : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::ENEMYSTATEMACHINE);

	enum StateType {
		WALKING, ATTACKING, DYING, INACTIVE
	};

	EnemyStateMachine(float dist) :  _dist(dist),_img(nullptr),_state(nullptr),_type(StateType::WALKING){ };

	virtual ~EnemyStateMachine() {};
	void update();
	void render() override;
	void handleInput() {} ;

	inline void setImg(Texture *img) {
		_img = img;
	}

	inline void setState(StateType s);
private:
	Texture *_img;
	float _dist;

	State* _state;
	StateType _type;
	//WalkingState* _walking_state;
	//static AttackingState* _attacking_state;
	//static DyingState* _dying_state;
	//static InactiveState* _inactive_state;

};

