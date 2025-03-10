#include "AttackingState.h"

#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/EnemyStateMachine.h"
#include "../components/Weapon.h"


AttackingState::AttackingState(Transform* tr, Transform* playerTr, Weapon* weapon, OnAttackCallback onAttackCallback) :
	_tr(tr), _playerTr(playerTr), _weapon(weapon), _onAttackCallback(onAttackCallback){
}

void AttackingState::enter() {
}

void AttackingState::update(uint32_t delta_time) {
	(void)delta_time;
	if (_tr == nullptr || _playerTr == nullptr ||_weapon==nullptr) return;
	
	Vector2D _target = _playerTr->getPos();
	_weapon->shoot(_target);

	if (_onAttackCallback) _onAttackCallback();
}

void AttackingState::exit() {
	
}
