#include "EnemyStateMachine.h"
#include "../states/WalkingState.h"
#include "../states/AttackingState.h"

inline void EnemyStateMachine::setState(StateType s) {
	if (_type != s) {
		delete _state;
		switch (s) {
		case WALKING:
			_state = new WalkingState();//_walking_state;
			break;
		case ATTACKING:
			_state = new AttackingState();//_attacking_state;
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
	_state->enter(_ent);
}