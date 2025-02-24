#include "State.h"

class InactiveState : public State
{
protected:
	Transform* _tr;
	Transform* _playerTr;
	Health* _health;
	EnemyStateMachine* _stateMachine;
public:
	InactiveState() {}

	virtual void enter(ecs::Entity* _enemy) {
		_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_enemy);
		_health = Game::Instance()->get_mngr()->getComponent<Health>(_enemy);
		_stateMachine = Game::Instance()->get_mngr()->getComponent<EnemyStateMachine>(_enemy);
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	}

	virtual void update() {
		_tr->setDir(Vector2D (0,0));
		if (/*Out of the pool*/) {
			_stateMachine->setState(BasicEnemy::WALKING);
		}
	}
private:
};