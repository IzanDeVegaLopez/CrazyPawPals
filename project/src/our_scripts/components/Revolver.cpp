#include "Revolver.h"
#include "../../our_scripts/Bullet.h"

Revolver::Revolver() : Weapon(4, 0.5f, 20.0f, 5.0f) {}

Revolver::~Revolver() {}

void 
Revolver::callback(Vector2D shootPos, Vector2D shootDir) {
	//_pool.push_back(new Bullet(_ent->getMngr(), shootPos, shootDir, Vector2D(0, 0), _speed, _distance));
}