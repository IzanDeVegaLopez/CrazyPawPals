#include "../our_scripts/states/State.h"

class AttackingState : public State
{
protected:
	Transform* _tr;
	Transform* _playerTr;
	Health* _health;
	//ShootComponent* _shoot;
	EnemyStateMachine* _stateMachine;
	float _dist;
public:
	AttackingState() {}

	virtual void enter(ecs::Entity* _enemy) {
		_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_enemy);
		_health = Game::Instance()->get_mngr()->getComponent<Health>(_enemy);
		//_shoot = Game::Instance()->get_mngr()->getComponent<ShootComponent>(_enemy);
		_stateMachine = Game::Instance()->get_mngr()->getComponent<EnemyStateMachine>(_enemy);
		_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	}

	virtual void update(ecs::Entity* _enemy) {
		Vector2D _target = _playerTr->getPos();
		//_shoot->shoot(_target);

		if (std::abs(_tr - _playerTr) > _dist) {
			_stateMachine->setState(EnemyStateMachine::WALKING);
		}

		if (_health->getHealth() <= 0) {
			_stateMachine->setState(EnemyStateMachine::INACTIVE);
		}
	}
};