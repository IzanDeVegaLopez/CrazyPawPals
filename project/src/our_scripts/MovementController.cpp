#include "MovementController.h"

#include "../sdlutils/SDLUtils.h"
#include "Container.h"

MovementController::MovementController() : _maxSpeed(5.0f), _reduceSpeed(0.05f) {
}

MovementController::~MovementController() {
}

void MovementController::update(Container* o)
{
	//Aceleración por tiempo, en 2 segundos, se pone a _maxSpeed
	//o->setSpeed( _maxSpeed);

	//Deceleración
	//if (_direction == contraria) {}
	//float deceleration = o->getSpeed() * _reduceSpeed;
	//o->setSpeed(deceleration);
}
