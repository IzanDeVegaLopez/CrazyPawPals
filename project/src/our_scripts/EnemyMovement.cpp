#include "EnemyMovement.h"
#include "../sdlutils/SDLUtils.h"
#include "Container.h"
EnemyMovement::EnemyMovement(Container* o): _target(o){

}

EnemyMovement::~EnemyMovement() {
}

void EnemyMovement::update(Container* o) {

	Vector2D _direction_target = _target->getPos() - o->getPos();
	float _ang_target = _direction_target.angle(Vector2D(1, 0));

	float angleDif = _ang_target - o->getRotation();

	float _vel_rot = 5.0f;

	// Si la diferencia de ángulo se sale de (-180, 180), se corrige
	// para que cambie el sentido de rotación
	if (abs(angleDif) > 180.0f) {
		angleDif = angleDif + (angleDif > 180.0f ? -1 : +1) * 360.0f;
	}

	// Si la diferencia de ángulo es menor que la velocidad de rotación, significa
	// que ya está en la posición correcta y se establece a la posición final
	if (abs(angleDif) < _vel_rot) {
		o->setRotation(_ang_target);
	} else {
		o->setRotation(o->getRotation() + (angleDif > 0 ? +1 : -1)*_vel_rot);
	}
}
