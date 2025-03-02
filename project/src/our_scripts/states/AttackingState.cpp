#include "AttackingState.h"

#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/EnemyStateMachine.h"
#include "../components/Weapon.h"


AttackingState::AttackingState(float dist, Transform* tr,
	Health* health, Weapon* weapon, EnemyStateMachine* stateMachine,
	Transform* playerTr) :
	_tr(tr), _health(health), _weapon(weapon),
	_stateMachine(stateMachine), _playerTr(playerTr), _dist(dist){
}

void AttackingState::enter() {
	/*_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	_health = Game::Instance()->get_mngr()->getComponent<Health>(_ent);
	_weapon = Game::Instance()->get_mngr()->getComponent<Weapon>(_ent);
	_stateMachine = Game::Instance()->get_mngr()->getComponent<EnemyStateMachine>(_ent);
	auto playerEntities = Game::Instance()->get_mngr()->getEntities(ecs::grp::PLAYER);
	if (!playerEntities.empty()) {
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(playerEntities[0]);
	}
	else {
		std::cerr << "Error: No se encontr� el jugador.\n";
	}*/
}

void AttackingState::update(uint32_t delta_time) {
	(void)delta_time;
	if (_tr == nullptr || _health == nullptr || _stateMachine == nullptr || _playerTr == nullptr ||_weapon==nullptr) return;
	
	Vector2D _target = _playerTr->getPos();
	_weapon->shoot(_target);

	if (std::abs(_tr - _playerTr) > _dist) {
		_stateMachine->setState(EnemyStateMachine::WALKING);
	}


	if (_health->getHealth() <= 0) {
		//_stateMachine->setState(EnemyStateMachine::INACTIVE);
	}
}

void AttackingState::exit() {
}
