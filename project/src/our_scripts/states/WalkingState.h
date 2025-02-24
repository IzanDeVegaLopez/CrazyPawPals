#include "State.h"

class WalkingState : public State
{
public:
	WalkingState() {}

	virtual void enter() {
		_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_basicEnemy._ent);
		_health = Game::Instance()->get_mngr()->getComponent<Health>(_basicEnemy._ent);
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	}

	virtual void update() override {

		Vector2D newDir = (_playerTr->getPos() - _tr->getPos()) * _tr->getSpeed();
		_tr->setDir(newDir);

		if (std::abs(_tr - _playerTr) < _dist) {
			_basicEnemy.setState(BasicEnemy::ATTACKING);
		}

		if (_health <= 0) {
			_basicEnemy.setState(BasicEnemy::DYING);
		}
	}
};