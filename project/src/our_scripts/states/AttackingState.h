#include "State.h"

class Transform;
class Health;
class Weapon;
class EnemyStateMachine;
class AttackingState : public State
{
protected:
	Transform* _tr;
	Health* _health;
	Weapon* _weapon;
	EnemyStateMachine* _stateMachine;
	Transform* _playerTr;
	float _dist;

public:
	AttackingState(float dist, Transform* tr,
		Health* health, Weapon* weapon,
		EnemyStateMachine* stateMachine, Transform* playerTr);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};