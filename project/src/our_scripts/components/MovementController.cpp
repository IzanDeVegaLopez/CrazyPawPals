#include "MovementController.h"

#include "../sdlutils/SDLUtils.h"
#include "Container.h"

MovementController::MovementController() : _maxSpeed(10.0f), _reduceSpeed(0.995f) {
}

MovementController::~MovementController() {
}

void MovementController::update(Container* o)
{
	//Aceleración por tiempo, en 2 segundos, se pone a _maxSpeed
	o->setSpeed( _maxSpeed);

	//Deceleración (si la actual es distinta a la anterior)
	//if (o->getDir() != contraria) {}
	//float deceleration = o->getSpeed() * _reduceSpeed;
	//o->setSpeed(deceleration);
}

void MovementController::accelerate(Container* o)
{

}


