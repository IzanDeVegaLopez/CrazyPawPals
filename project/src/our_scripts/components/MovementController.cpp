#include "MovementController.h"
#include "Transform.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../game/Game.h"

MovementController::MovementController() : _tr(nullptr) {
}

MovementController::~MovementController() {
}

void
MovementController::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void MovementController::set_input(Vector2D vec) {
	_input = vec.normalize();
}

void MovementController::update(uint32_t delta_time)
{
	Vector2D expected_speed = _input * _maxSpeed;

	Vector2D speed_dif = expected_speed - _tr->getDir();
	//calculamos si usar aceleraci�n o deceleraci�n
	float accelRate = (expected_speed.magnitude() - _tr->getDir().magnitude() < 0 || abs(expected_speed.angle(_tr->getDir())) > 15) ? _decceleration : _acceleration;

	accelRate *= delta_time /1000.0f ;
	//std::cout << _tr->getSpeed() << std::endl;
	_tr->add_directional_speed(speed_dif * accelRate);

	/*
	Vector2D my_new_speed = _tr->getDir() * (1 - _decceleration) + expected_speed * _decceleration;
	float accelerate = _acceleration * delta_time/1000;
	//std::cout << "acc  "<< accelerate << std::endl;
	if (my_new_speed.magnitude() > _tr->getSpeed()) {
		//accelerate
		_tr->setDir(_tr->getDir() * (1 - accelerate) + expected_speed * accelerate);
	}
	else {

		float deccelerate = _decceleration * delta_time/1000;
		//std::cout << "dec  " << deccelerate << std::endl;
		//deccelerate
		_tr->setDir(_tr->getDir() * (1 - deccelerate) + expected_speed * deccelerate);
	}
	*/
}



