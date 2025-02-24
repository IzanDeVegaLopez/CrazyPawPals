#include "State.h"

class AttackingState : public State
{
protected:
	Transform* _tr;
	Transform* _playerTr;
	Health* _health;
	Weapon* _weapon;
	EnemyStateMachine* _stateMachine;
	float _dist;
public:
	AttackingState() {}

	void enter(ecs::Entity* _enemy) {
		_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_enemy);
		_health = Game::Instance()->get_mngr()->getComponent<Health>(_enemy);
		_weapon = Game::Instance()->get_mngr()->getComponent<Weapon>(_enemy);
		_stateMachine = Game::Instance()->get_mngr()->getComponent<EnemyStateMachine>(_enemy);
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	}

	void update() {
		Vector2D _target = _playerTr->getPos();
		_weapon->shoot(_target);

		if (std::abs(_tr - _playerTr) > _dist) {
			_stateMachine->setState(EnemyStateMachine::WALKING);
		}

		if (_health->getHealth() <= 0) {
			_stateMachine->setState(EnemyStateMachine::INACTIVE);
		}
	}
	void exit() {};
};