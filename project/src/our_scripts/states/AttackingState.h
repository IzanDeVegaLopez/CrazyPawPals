#include "State.h"

#include <functional>
class Transform;
class Weapon;
class AttackingState : public State
{
public:
	using OnAttackCallback = std::function<void()>;

	AttackingState(Transform* tr, Transform* playerTr, Weapon* weapon, OnAttackCallback onAttackCallback = nullptr, int attact_times = 1);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;

protected:
	Transform* _tr;
	Transform* _playerTr;
	Weapon* _weapon;
	OnAttackCallback _onAttackCallback;

	int _attack_times;
	int _contador;
};