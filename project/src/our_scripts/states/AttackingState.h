#include "State.h"

class Transform;
class Health;
class Weapon;
class EnemyStateMachine;
class AttackingState : public State
{
protected:
	Transform* _tr;
	Transform* _playerTr;
	Health* _health;
	Weapon* _weapon;
	EnemyStateMachine* _stateMachine;
	float _dist;

public:
	AttackingState(float);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};