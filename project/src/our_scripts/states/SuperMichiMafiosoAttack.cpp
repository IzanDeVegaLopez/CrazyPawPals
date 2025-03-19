#include "SuperMichiMafiosoAttack.h"

#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/WeaponSuperMichiMafioso.h"


SuperMichiMafiosoAttack::SuperMichiMafiosoAttack(Transform* tr, Transform* playerTr, WeaponSuperMichiMafioso* weapon, OnChangeAttack onChangeAttack) :
	_tr(tr), _playerTr(playerTr), _weapon(weapon), _onChangeAttack(onChangeAttack) {
}

void SuperMichiMafiosoAttack::enter() {
}

void SuperMichiMafiosoAttack::update(uint32_t delta_time) {
	(void)delta_time;
	if (_tr == nullptr || _playerTr == nullptr || _weapon == nullptr) return;

	if (_onChangeAttack) _onChangeAttack();
	Vector2D _target = _playerTr->getPos();
	_weapon->shoot(_target);
}

void SuperMichiMafiosoAttack::exit() {

}
