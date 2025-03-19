#include "MovementController.h"
#include "Transform.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../game/Game.h"

MovementController::MovementController(float max_speed, float acceleration, float decceleration) 
	: _tr(nullptr), _max_speed(max_speed), _acceleration(acceleration), _decceleration(decceleration), _dashing(false), _time_remaining(0){
}

MovementController::~MovementController() {
}

void
MovementController::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void MovementController::set_input(Vector2D vec) {
	if (!_dashing) {
		_input = vec.normalize();
	}
}

void MovementController::update(uint32_t delta_time)
{
	if (_dashing) {
		Vector2D currentPosition = _tr->getPos();
		Vector2D direction = (_dash_pos - currentPosition).normalize();
		float distanceToTarget = (_dash_pos - currentPosition).magnitude();

		float speed = distanceToTarget / (_time_remaining / 1000.0f);

		_tr->setPos(currentPosition + direction * speed * (delta_time / 1000.0f));

		_time_remaining -= delta_time;

		// Si el tiempo de dash ha terminado
		if (_time_remaining <= 100 || distanceToTarget <= 0.1f) {
			_dashing = false;
			_tr->setPos(_dash_pos);
			_tr->setDir(Vector2D(0, 0));
		}
	}
	else {
		Vector2D expected_speed = _input * _max_speed;
		Vector2D speed_dif = expected_speed - _tr->getDir();
		//calculamos si usar aceleraci�n o deceleraci�n
		float accelRate = (expected_speed.magnitude() - _tr->getDir().magnitude() < 0 || abs(expected_speed.angle(_tr->getDir())) > 15) ? _decceleration : _acceleration;
		accelRate *= delta_time / 1000.0f;
		_tr->add_directional_speed(speed_dif * accelRate);
	}

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


void MovementController::dash(Vector2D next_pos, uint32_t time) {
	if (!_dashing) {
		// Solo podemos iniciar un dash si no estamos ya en uno
		_dashing = true;
		_time_remaining = time;
		_dash_pos = next_pos;
	}
}

