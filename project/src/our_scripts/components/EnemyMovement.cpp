#include "EnemyMovement.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Manager.h"

#include "Transform.h"


EnemyMovement::EnemyMovement(){

}

EnemyMovement::~EnemyMovement() {
}

void
EnemyMovement::initComponent() {
	auto* mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	_playerTr = mngr->getComponent<Transform>(mngr->getEntities(ecs::hdlr::PLAYER)[0]);
	assert(_tr != nullptr);
	assert(_playerTr != nullptr);
}

void 
EnemyMovement::update(int delta_time) {

	Vector2D newDir = _playerTr->getPos() - _tr->getPos();

	newDir = newDir.normalize();

	if (newDir.magnitude() > 0) {
		newDir = newDir.normalize();

		_tr->setDir(newDir * _tr->getSpeed());
	}
	else {
		_tr->setDir(Vector2D(0,0));
	}
}
