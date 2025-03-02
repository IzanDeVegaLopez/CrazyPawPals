// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../../ecs/Component.h"
#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/Weapon.h"

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

	EnemyStateMachine(float dist);

	virtual ~EnemyStateMachine() {};
	void update(uint32_t delta_time) override;
	void render() override;
	void initComponent() override;

	inline void setImg(Texture *img) {
		_img = img;
	}

	void setState(StateType s);
protected:
	StateType _type;
	Texture *_img;

	float _dist;
	/*Transform* _tr;
	Health* _health;
	Weapon* _weapon;
	Transform* _playerTr;*/

	State* _state;
	WalkingState* _walking_state;
	AttackingState* _attacking_state;
	//static DyingState* _dying_state;
	//static InactiveState* _inactive_state;
};

