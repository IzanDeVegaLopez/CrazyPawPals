#include "State.h"

class Transform;
class Health;
class EnemyStateMachine;
class entity_t;
class WalkingState : public State
{
protected:
	EnemyStateMachine* _stateMachine;
	Transform* _tr;
	Transform* _playerTr;
	Health* _health;
	float _dist;
public:
	WalkingState(float dist, Transform* tr,
		Health* health,	EnemyStateMachine* stateMachine,
		Transform* playerTr);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};
