#include "WalkingState.h"

#include "../../game/scenes/GameScene.h"
#include "../components/movement/Transform.h"
#include "../components/movement/Follow.h"
#include "../components/movement/MovementController.h"

WalkingState::WalkingState(Transform *tr, MovementController *movementController,Follow* follow, bool toDestination)
    : _tr(tr), _movement_controller(movementController), _fll(nullptr), _to_destination(toDestination)
{

}

void WalkingState::enter()
{
    // assert(_tr != nullptr);
    // assert(_movementController != nullptr);

    // assert(_player_tr != nullptr);
}

void WalkingState::update(uint32_t delta_time)
{
    (void)delta_time;

    // Calcular la dirección hacia el jugador
    Vector2D direction = _fll->get_act_follow()->getPos() - _tr->getPos();
    direction = direction.normalize();

    // Establecer la dirección en el MovementController
    _movement_controller->set_input(_to_destination ? (direction * -1.0f) : direction);
}

void WalkingState::exit()
{
    _movement_controller->set_input({0, 0});
}