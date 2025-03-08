#include "AttackingState.h"

#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/EnemyStateMachine.h"
#include "../components/Weapon.h"


AttackingState::AttackingState(Transform* tr, Transform* playerTr, Weapon* weapon) :
	_tr(tr), _playerTr(playerTr), _weapon(weapon){
}

void AttackingState::enter() {
    //std::cout << "Entering AttackingState" << std::endl;

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
	if (_tr == nullptr || _playerTr == nullptr ||_weapon==nullptr) return;
	
	Vector2D _target = _playerTr->getPos();
	_weapon->shoot(_target);
}

void AttackingState::exit() {
	//std::cout << "Exiting AttackingState" << std::endl;
	
}
