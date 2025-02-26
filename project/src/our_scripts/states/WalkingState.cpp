#include "WalkingState.h"

#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/EnemyStateMachine.h"

WalkingState::WalkingState(float dist) : _tr(nullptr), _health(nullptr),  _stateMachine(nullptr), _playerTr(nullptr), _dist(dist) {
	
}

void WalkingState::enter() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	_health = Game::Instance()->get_mngr()->getComponent<Health>(_ent);
	_stateMachine = Game::Instance()->get_mngr()->getComponent<EnemyStateMachine>(_ent);
	auto playerEntities = Game::Instance()->get_mngr()->getEntities(ecs::grp::PLAYER);
	if (!playerEntities.empty()) {
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(playerEntities[0]);
	}
	else {
		std::cerr << "Error: No se encontró el jugador.\n";
	}
}

void WalkingState::update(uint32_t delta_time) {
	if (_tr == nullptr || _health == nullptr || _stateMachine == nullptr || _playerTr == nullptr) {
		//std::cerr << "Error: Componentes no inicializados en WalkingState::update()\n";
		return;
	}
	Vector2D newDir = (_playerTr->getPos() - _tr->getPos()) * _tr->getSpeed();
	_tr->setDir(newDir);

	if (std::abs(_tr - _playerTr) < _dist) {
		_stateMachine->setState(EnemyStateMachine::ATTACKING);
	}

	if (_health->getHealth() <= 0) {
		_stateMachine->setState(EnemyStateMachine::INACTIVE);
	}
}

void WalkingState::exit() {
}


