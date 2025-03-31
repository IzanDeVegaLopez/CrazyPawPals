#include "WalkingTotemState.h"

#include "../../game/scenes/GameScene.h"
#include "../components/Health.h"
#include "../components/movement/Transform.h"
#include "../components/movement/MovementController.h"

WalkingTotemState::WalkingTotemState(Transform* tr, MovementController* movementController, bool toDestination)
    : _tr(tr), _targetPos(Vector2D{0.0f,0.0f}), _movementController(movementController), _to_destination(toDestination) {}

void WalkingTotemState::enter() {
    //std::cout << "Entering WalkingState" << std::endl;
    // Lógica de entrada
    _targetPos = Vector2D{ (float)sdlutils().rand().nextInt(-20, 21), (float)sdlutils().rand().nextInt(-15, 16) };
}

void WalkingTotemState::update(uint32_t delta_time) {
    //std::cout << "Updating WalkingState" << std::endl;
    if (abs((_targetPos - _tr->getPos()).magnitude()) <= 2.0f) {
        _targetPos = Vector2D{ (float)sdlutils().rand().nextInt(-20, 21), (float)sdlutils().rand().nextInt(-15, 16) };
    }
    // Calcular la dirección hacia el jugador
    Vector2D direction = _targetPos - _tr->getPos();
    direction = direction.normalize();

    // Establecer la dirección en el MovementController
	_movementController->set_input(_to_destination ? (direction * -1.0f) : direction);
}

void WalkingTotemState::exit() {
   //std::cout << "Exiting WalkingState" << std::endl;

   _movementController->set_input({0,0});
    
}