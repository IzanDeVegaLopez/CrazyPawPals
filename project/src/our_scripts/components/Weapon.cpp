#include "Weapon.h"
#include "../../sdlutils/SDLUtils.h"

Weapon::Weapon(int d, float cd, float dis, float speed) : _damage(d), _cooldown(cd), _distance(dis), _speed(speed){}

Weapon::~Weapon() {}
