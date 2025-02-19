#include "MovementController.h"
#include "Transform.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../game/Game.h"

MovementController::MovementController() : _maxSpeed(5.0f), _reduceSpeed(0.5f), _addSpeed(1.025f) {
}

MovementController::~MovementController() {
}

void
MovementController::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
void MovementController::update(uint32_t delta_time)
{
	auto& dir = _tr->getDir();
	auto& prevDir = _tr->getPrevDir();
	auto& pos = _tr->getPos();
	auto speed = _tr->getSpeed();

	//We change the position accoding to the inputs we've recieved from keyboard control
	if (dir.getX() != 0 && dir.getY() != 0) {
		dir = dir.normalize(); //If its a diagonal movement, normalize dir
	}

	//Acceleration (only if dir != 0,0 and same direction)
	if (dir != Vector2D(0, 0) && dir == prevDir){
		speed *= _addSpeed;
		if (speed > _maxSpeed) {
			speed = _maxSpeed;
		}
		_tr->setSpeed(speed);
	}

	//Deacceleration (on change direction or stop movement)
	if ((dir == Vector2D(0, 0) && dir != prevDir) || dir != prevDir) {
		speed *= _reduceSpeed;
		_tr->setSpeed(speed);
	}

}


