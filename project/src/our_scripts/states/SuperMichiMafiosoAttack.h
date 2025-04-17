#include "State.h"
#include <functional>
class Transform;
class WeaponSuperMichiMafioso;

class SuperMichiMafiosoAttack : public State
{
public:
	using OnChangeAttack = std::function<void()>;

	SuperMichiMafiosoAttack(Transform* tr, Transform* playerTr, WeaponSuperMichiMafioso* weapon, OnChangeAttack onChangeAttack = nullptr);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;

protected:
	Transform* _tr;
	Transform* _player_tr;
	WeaponSuperMichiMafioso* _weapon;
	OnChangeAttack _onChangeAttack;
};