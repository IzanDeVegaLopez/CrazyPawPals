// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../states/State.h"
//#include "../states/DyingState.h"
//#include "../states/InactiveState.h"

class Texture;
class WalkingState;
class AttackingState;
class EnemyStateMachine : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::ENEMYSTATEMACHINE);

	enum StateType {
		WALKING, ATTACKING, DYING, INACTIVE
	};

	EnemyStateMachine(float dist) { _dist == dist; };

	virtual ~EnemyStateMachine();
	void update() { _state->update(); };
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

