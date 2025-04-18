#include "SuperMichiMafiosoAttack.h"

#include "../../game/scenes/GameScene.h"
#include "../components/Health.h"
#include "../components/movement/Transform.h"
#include "../components/weapons/enemies/WeaponSuperMichiMafioso.h"


SuperMichiMafiosoAttack::SuperMichiMafiosoAttack(Transform* tr, WeaponSuperMichiMafioso* weapon, OnChangeAttack onChangeAttack) :
	_tr(tr), _weapon(weapon), _onChangeAttack(onChangeAttack) {
}

void SuperMichiMafiosoAttack::enter() {
}

void SuperMichiMafiosoAttack::update(uint32_t delta_time) {
	(void)delta_time;
	if (_tr == nullptr || _player_tr == nullptr || _weapon == nullptr) return;

	if (_onChangeAttack) _onChangeAttack();
	Vector2D _target = _player_tr->getPos();
	_weapon->shoot(_target);
}

void SuperMichiMafiosoAttack::exit() {

}
