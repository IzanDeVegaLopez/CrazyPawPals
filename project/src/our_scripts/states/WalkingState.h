#include "State.h"
class EnemyStateMachine;
class WalkingState : public State
{
protected:
	Transform* _tr;
	Transform* _playerTr;
	Health* _health;
	EnemyStateMachine* _stateMachine;
public:
	WalkingState() {}

	virtual void enter(ecs::Entity* _enemy) {
		_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_enemy);
		_health = Game::Instance()->get_mngr()->getComponent<Health>(_enemy);
		_stateMachine = Game::Instance()->get_mngr()->getComponent<EnemyStateMachine>(_enemy);
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	}

	virtual void update() override {

		Vector2D newDir = (_playerTr->getPos() - _tr->getPos()) * _tr->getSpeed();
		_tr->setDir(newDir);

		if (std::abs(_tr - _playerTr) < _dist) {
			_stateMachine->setState(BasicEnemy::ATTACKING);
		}

		if (_health <= 0) {
			_stateMachine->setState(BasicEnemy::DYING);
		}
	}
};