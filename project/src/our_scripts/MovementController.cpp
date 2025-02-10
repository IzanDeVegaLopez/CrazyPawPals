#include "MovementController.h"

#include "../sdlutils/SDLUtils.h"
#include "Container.h"

MovementController::MovementController() : _maxSpeed(5.0f), _reduceSpeed(0.05f), _direction(0.0f, 1.0f) {
}

MovementController::~MovementController() {
}

void MovementController::update(Container* o)
{
	Vector2D movementSpeed = o->getVel() * _direction * _maxSpeed;

	if (movementSpeed.magnitude() > _maxSpeed) {
		movementSpeed = movementSpeed.normalize() * _maxSpeed;
	}
	o->getVel().setX(movementSpeed.getX());
	o->getVel().setY(movementSpeed.getY());

	if (_direction = )
	Vector2D deceleration = o->getVel() * _reduceSpeed;
	o->setVel(deceleration);
}
