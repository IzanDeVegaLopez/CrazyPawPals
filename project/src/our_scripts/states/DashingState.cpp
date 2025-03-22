#include "DashingState.h"

#include "../../game/scenes/GameScene.h"
#include "../components/movement/Transform.h"
#include "../components/movement/MovementController.h"

DashingState::DashingState(Transform* tr, Transform* playerTr, MovementController* movementController, bool toDestination, uint32_t time, float extra_space)
    : _tr(tr), _playerTr(playerTr), _movementController(movementController), _to_destination(toDestination), _time(time), _extra_space(extra_space){}

void DashingState::enter() {
    //std::cout << "Entering DashingState" << std::endl;

    Vector2D direction = _playerTr->getPos() - _tr->getPos();
    direction = direction.normalize();

    Vector2D dash_target = _playerTr->getPos() + direction * _extra_space;
    _movementController->dash(dash_target, _time);
}

void DashingState::update(uint32_t delta_time) {
    //std::cout << "Updating DashingState" << std::endl;
}

void DashingState::exit() {
   _movementController->set_input({0,0});
}