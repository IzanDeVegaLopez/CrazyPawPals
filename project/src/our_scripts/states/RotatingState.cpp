#include "RotatingState.h"
#include "../components/movement/Transform.h"
#include "../components/movement/MovementController.h"

RotatingState::RotatingState(Transform* tr, Transform* playerTr, MovementController* mc)
    : _tr(tr), _movementController(mc), _playerTr(playerTr), _angle(0.0f), _center(), _radius(0.0f) {}
void
RotatingState::enter() {
    _center = { _playerTr->getPos().getX() + (_playerTr->getWidth() / 2),_playerTr->getPos().getY() + (_playerTr->getHeight() / 2) };
    _radius = (_tr->getPos() - _center).magnitude();
    _angle = std::atan2(_tr->getPos().getY() - _center.getY(), _tr->getPos().getX() - _center.getX());
}
void
RotatingState::update(uint32_t delta_time) {

    _angle += _ANGULAR_SPEED * (delta_time / 1000.0f);

    Vector2D newPosition = _center + Vector2D(_radius * std::cos(_angle), _radius * std::sin(_angle));

    Vector2D moveDirection = newPosition - _tr->getPos();

    _movementController->set_input(newPosition - _tr->getPos());

    _tr->setPos(newPosition);
}
void RotatingState::exit() {};
