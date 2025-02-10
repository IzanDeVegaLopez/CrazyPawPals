#include "EnemyMovement.h"
#include "../sdlutils/SDLUtils.h"
#include "Container.h"
EnemyMovement::EnemyMovement(Container* o): _target(o){

}

EnemyMovement::~EnemyMovement() {
}

void EnemyMovement::update(Container* o) {

	Vector2D dif = _target->getPos() - o->getPos();

	float _ang = dif.angle(o->getVel());

	//o->setRotation(
}
