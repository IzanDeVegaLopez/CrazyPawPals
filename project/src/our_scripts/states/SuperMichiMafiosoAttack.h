#include "State.h"

#include <functional>
class Transform;
class WeaponSuperMichiMafioso;
class MovementController;
class SuperMichiMafiosoAttack : public State
{
public:

	SuperMichiMafiosoAttack(Transform* tr, Transform* playerTr, WeaponSuperMichiMafioso* weapon, MovementController* mc);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;

protected:
	Transform* _tr;
	Transform* _playerTr;
	WeaponSuperMichiMafioso* _weapon;
	MovementController* _mc;
	enum Pattern { PATTERN_NONE, PATTERN1, PATTERN2, PATTERN3 };
	Pattern _currentPattern;
	float _attackCount;
	float _timer;

};