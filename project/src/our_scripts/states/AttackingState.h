#include "State.h"

class Transform;
class Weapon;
class AttackingState : public State
{
protected:
	Transform* _tr;
	Transform* _playerTr;
	Weapon* _weapon;
public:
	AttackingState(Transform* tr, Transform* playerTr, Weapon* weapon);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};