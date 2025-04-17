#include "WalkingState.h"

#include "../../game/scenes/GameScene.h"
#include "../components/Health.h"
#include "../components/movement/Transform.h"
#include "../components/movement/MovementController.h"

WalkingState::WalkingState(Transform *tr, MovementController *movementController, bool toDestination)
    : _tr(tr), _player_tr(nullptr), _movementController(movementController), _to_destination(toDestination)
{
    assert(_tr != nullptr);
    assert(_movementController != nullptr);

    assert(_player_tr != nullptr);
}

void WalkingState::enter()
{
    assert(_tr != nullptr);
    assert(_movementController != nullptr);
    search_player_tr();
    assert(_player_tr != nullptr);
}

void WalkingState::update(uint32_t delta_time)
{
    (void)delta_time;

    // Calcular la dirección hacia el jugador
    Vector2D direction = _player_tr->getPos() - _tr->getPos();
    direction = direction.normalize();

    // Establecer la dirección en el MovementController
    _movementController->set_input(_to_destination ? (direction * -1.0f) : direction);
}

void WalkingState::exit()
{

    _movementController->set_input({0, 0});
}