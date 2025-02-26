#include "EnemyStateMachine.h"
#include "../states/State.h"
#include "../states/WalkingState.h"
#include "../states/AttackingState.h"
#include <iostream>

EnemyStateMachine::EnemyStateMachine(float dist) :_dist(dist), _img(nullptr), _state(nullptr), _type(StateType::WALKING), _walking_state(nullptr), _attacking_state(nullptr)
{

};

void EnemyStateMachine::initComponent() {
	
	auto playerEntities = Game::Instance()->get_mngr()->getEntities(ecs::grp::PLAYER);

	_walking_state = new WalkingState(_dist,
		Game::Instance()->get_mngr()->getComponent<Transform>(_ent),
		Game::Instance()->get_mngr()->getComponent<Health>(_ent), this,
		Game::Instance()->get_mngr()->getComponent<Transform>(playerEntities[0]));
	_attacking_state = new AttackingState(_dist,
		Game::Instance()->get_mngr()->getComponent<Transform>(_ent),
		Game::Instance()->get_mngr()->getComponent<Health>(_ent),
		Game::Instance()->get_mngr()->getComponent<Weapon>(_ent), this,
		Game::Instance()->get_mngr()->getComponent<Transform>(playerEntities[0]));

	_attacking_state->setEntity(_ent);
	_walking_state->setEntity(_ent);

	_state = _walking_state;
}

void EnemyStateMachine::update(uint32_t delta_time) {
	_state->update(delta_time);
	if (_state == _walking_state) {
		std::cout << "Walking";
	}
	else if (_state == _attacking_state) {
		std::cout << "Attacking";

	}
}
void EnemyStateMachine::render() {
}
void EnemyStateMachine::setState(StateType s) {
	if (_type != s) {
		switch (s) {
		case WALKING:
			_state = _walking_state;//_walking_state;
			break;
		case ATTACKING:
			_state = _attacking_state;//_attacking_state;
			break;
			/*case DYING:
				_state = _dying_state;
				break;
			case INACTIVE:
				_state = new InactiveState();
				break;*/
		default:
			assert(false);
			break;
		}
	}

	_type = s;
	_state->enter();
}