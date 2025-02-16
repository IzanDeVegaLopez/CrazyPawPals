#include "MovementController.h"
#include "Transform.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"

MovementController::MovementController() : _maxSpeed(5.0f), _reduceSpeed(0.75f), _addSpeed(1.01f) {
}

MovementController::~MovementController() {
}

void
MovementController::initComponent() {
	auto* mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
void MovementController::update()
{
	//Deceleración (si la actual es distinta a la anterior)
	//if (o->getDir() != contraria) {}
	//float deceleration = o->getSpeed() * _reduceSpeed;
	//o->setSpeed(deceleration);
	auto& dir = _tr->getDir();
	auto& prevDir = _tr->getPrevDir();
	auto& pos = _tr->getPos();
	auto speed = _tr->getSpeed();

	/*
	//We change the position accoding to the inputs we've recieved from keyboard control
	if (dir.getX() != 0 && dir.getY() != 0) {
		dir = dir.normalize(); //If its a diagonal movement, normalize dir
	}*/

	//Acceleration
	if (dir != Vector2D(0, 0) && dir == prevDir){
		speed *= _addSpeed;
		if (speed > _maxSpeed) {
			speed = _maxSpeed;
		}
		_tr->setSpeed(speed);
	}

	//Deacceleration on change direction or stop movement
	if ((dir == Vector2D(0, 0) && dir != prevDir) || dir != prevDir) {
		speed *= _reduceSpeed;
		_tr->setSpeed(speed);
	}

	//New position for movement
	pos = pos + (dir * speed);
}


