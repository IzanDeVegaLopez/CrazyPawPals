#include "WalkingState.h"

#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/MovementController.h"

WalkingState::WalkingState(Transform* tr, Transform* playerTr, MovementController* movementController, bool toDestination)
    : _tr(tr), _playerTr(playerTr), _movementController(movementController), _to_destination(toDestination) {}

void WalkingState::enter() {
    //std::cout << "Entering WalkingState" << std::endl;
    // Lógica de entrada
}

void WalkingState::update(uint32_t delta_time) {
    //std::cout << "Updating WalkingState" << std::endl;

    // Calcular la dirección hacia el jugador
    Vector2D direction = _playerTr->getPos() - _tr->getPos();
    direction = direction.normalize();

    // Establecer la dirección en el MovementController
	_movementController->set_input(_to_destination ? (direction * -1.0f) : direction);
}

void WalkingState::exit() {
   //std::cout << "Exiting WalkingState" << std::endl;

   _movementController->set_input({0,0});
    
}