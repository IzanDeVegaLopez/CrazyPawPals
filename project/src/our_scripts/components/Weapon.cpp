#include "Weapon.h"
#include "../../sdlutils/SDLUtils.h"

Weapon::Weapon() : _damage(0), _cooldown(0.0f), _distance(0.0f), _speed(0.0f){}

Weapon::~Weapon() {}

void 
Weapon::init(int d, float cd, float dis, float s) {
	_damage = d;
	_cooldown = cd;
	_distance = dis;
	_speed = s;
}

