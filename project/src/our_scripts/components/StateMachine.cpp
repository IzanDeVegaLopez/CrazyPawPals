#include "StateMachine.h"
#include "../states/State.h"
#include "../states/Conditions.h"
#include "../states/AttackingState.h"
#include "../states/WalkingState.h"
#include "../components/Transform.h"

#include <iostream>

StateMachine::StateMachine(ConditionManager& conditionManager): _condition_manager(conditionManager) {
	
}


void StateMachine::add_state(const std::string& name, StatePtr state) {
	_states[name] = state;
}

void StateMachine::add_transition(const std::string& from, const std::string& to, TransitionCondition condition) {
	_transitions[from].push_back({to, condition});
}

void StateMachine::set_initial_state(const std::string& name) {
	_currentState = name;
	if (_states.count(_currentState)) {
		_states[_currentState]->enter();
	}
}

void StateMachine::update(uint32_t delta_time) {
	if (_states.count(_currentState)) {
		_states[_currentState]->update(delta_time);

		for (const auto& transition : _transitions[_currentState]) {
			if (transition.condition()) {
				_states[_currentState]->exit();
				_currentState = transition.targetState;
				_states[_currentState]->enter();
				break;
			}
		}
	}
}

void StateMachine::enter() {
	if (_states.count(_currentState)) {
		_states[_currentState]->enter();
	}
}

void StateMachine::exit() {
	if (_states.count(_currentState)) {
		_states[_currentState]->exit();
	}
}

