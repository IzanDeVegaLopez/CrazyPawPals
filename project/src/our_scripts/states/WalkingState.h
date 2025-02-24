#include "State.h"

class WalkingState : public State
{
protected:
	EnemyStateMachine* _stateMachine;
	Transform* _tr;
	Transform* _playerTr;
	Health* _health;
	float _dist;
public:
	WalkingState() {}

	void enter(ecs::Entity* _enemy) {
		_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_enemy);
		_health = Game::Instance()->get_mngr()->getComponent<Health>(_enemy);
		_stateMachine = Game::Instance()->get_mngr()->getComponent<EnemyStateMachine>(_enemy);
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	}

	void update() override {

		Vector2D newDir = (_playerTr->getPos() - _tr->getPos()) * _tr->getSpeed();
		_tr->setDir(newDir);

		if (std::abs(_tr - _playerTr) < _dist) {
			_stateMachine->setState(EnemyStateMachine::ATTACKING);
		}

		if (_health->getHealth() <= 0) {
			_stateMachine->setState(EnemyStateMachine::INACTIVE);
		}
	}
	void exit() {};
};