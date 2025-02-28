#include "EnemyMovement.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Manager.h"
#include "../../game/Game.h"

#include "Transform.h"


EnemyMovement::EnemyMovement(){

}

EnemyMovement::~EnemyMovement() {
}

void
EnemyMovement::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	assert(_tr != nullptr);
	assert(_playerTr != nullptr);
}

void 
EnemyMovement::update(uint32_t delta_time) {
	(void)delta_time;
	auto newDir = _playerTr->getPos() - _tr->getPos();

	newDir = newDir.normalize();

	if (true) {
		newDir = newDir.normalize();

		Vector2D tempDir = newDir * _tr->getSpeed();
		_tr->setDir(tempDir);

	}
	else {
		Vector2D tempDir(0,0);
		_tr->setDir(tempDir);

	}
}
