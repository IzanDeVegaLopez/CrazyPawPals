#include "State.h"
#include <functional>
class Transform;
class WeaponSuperMichiMafioso;
class Follow;

class SuperMichiMafiosoAttack : public State
{
public:
	using OnChangeAttack = std::function<void()>;

	SuperMichiMafiosoAttack(Transform* tr, WeaponSuperMichiMafioso* weapon, Follow* follow, OnChangeAttack onChangeAttack = nullptr);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;

protected:
	Transform* _tr;
	WeaponSuperMichiMafioso* _weapon;
	Follow* _fll;
	OnChangeAttack _onChangeAttack;
};