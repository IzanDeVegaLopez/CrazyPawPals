#include "SuperMichiMafiosoAttack.h"
#include "../../game/scenes/GameScene.h"
#include "../components/movement/Transform.h"
#include "../components/weapons/enemies/WeaponSuperMichiMafioso.h"
#include "../components/movement/Follow.h"


SuperMichiMafiosoAttack::SuperMichiMafiosoAttack(Transform* tr, WeaponSuperMichiMafioso* weapon, Follow* follow, OnChangeAttack onChangeAttack) :
	_tr(tr), _weapon(weapon), _fll(follow), _onChangeAttack(onChangeAttack) {
}

void SuperMichiMafiosoAttack::enter() {
}

void SuperMichiMafiosoAttack::update(uint32_t delta_time) {
	(void)delta_time;
	if (_tr == nullptr || _fll == nullptr || _weapon == nullptr) return;

	if (_onChangeAttack) _onChangeAttack();
	Vector2D _target = _fll->get_act_follow()->getPos();
	_weapon->shoot(_target);
}

void SuperMichiMafiosoAttack::exit() {

}
