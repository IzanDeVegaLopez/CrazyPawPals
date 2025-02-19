#include "Revolver.h"
#include "../../our_scripts/Bullet.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/Texture.h"
#include "../Bullet.h" 

Revolver::Revolver() : Weapon() { }

Revolver::~Revolver() {}

void 
Revolver::callback(Vector2D shootPos, Vector2D shootDir) {

	_pool.push_back(new Bullet(_ent->getMngr(), shootPos, shootDir, Vector2D(0, 0), _speed, _distance, _tex, _attack_width, _attack_height));
}