#include "DashingState.h"

#include "../../game/scenes/GameScene.h"
#include "../components/movement/Transform.h"
#include "../components/movement/MovementController.h"

DashingState::DashingState(Transform* tr, Transform* playerTr, MovementController* movementController, bool toDestination, uint32_t time, float extra_space)
    : _tr(tr), _player_tr(playerTr), _movementController(movementController), _to_destination(toDestination), _time(time), _extra_space(extra_space){}

void 
DashingState::enter() {
    

    Vector2D direction = _player_tr->getPos() - _tr->getPos();
    direction = direction.normalize();

    Vector2D dash_target = _player_tr->getPos() + direction * _extra_space;
    _movementController->dash(dash_target, _time);
}

void 
DashingState::update(uint32_t delta_time) {
    (void)delta_time;
    
}

void 
DashingState::exit() {
   _movementController->set_input({0,0});
}