#include "MovementController.h"
#include "Transform.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"

MovementController::MovementController() : _maxSpeed(10.0f), _reduceSpeed(0.995f) {
}

MovementController::~MovementController() {
}

void
MovementController::initComponent() {
	auto* mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
void MovementController::update(int delta_time)
{
	//Aceleración por tiempo, en 2 segundos, se pone a _maxSpeed
	//o->setSpeed( _maxSpeed);

	//Deceleración (si la actual es distinta a la anterior)
	//if (o->getDir() != contraria) {}
	//float deceleration = o->getSpeed() * _reduceSpeed;
	//o->setSpeed(deceleration);
	auto& dir = _tr->getDir();
	auto& pos = _tr->getPos();
	auto speed = _tr->getSpeed();

	//We change the position accoding to the inputs we've recieved from keyboard control
	if (dir.getX() != 0 && dir.getY() != 0) {
		dir = dir.normalize(); //If its a diagonal movement, normalize dir
	}
	pos = pos + (dir * speed);
}

void MovementController::accelerate()
{

}


