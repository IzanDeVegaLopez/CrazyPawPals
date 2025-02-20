#include "Weapon.h"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/virtualTimer.h"
#include "Transform.h"

//Weapon::Weapon() : _damage(0), _cooldown(0.0f), _distance(0.0f), _speed(0.0f), _tex(""), _attack_width(50), _attack_height(50) {}

Weapon::Weapon(int d, float cd, float dis, float s, const std::string& t)
	: _damage(d), _cooldown(cd), _distance(dis), _speed(s), _tex(t), _attack_width(50), _attack_height(50)
{
}

Weapon::~Weapon() {}

void 
Weapon::set_attack_size(int w, int h) {
	_attack_height = h; 
	_attack_width = w;
}
void
Weapon::shoot(const Vector2D& target) {
	auto& pos = _tr->getPos();
	if (sdlutils().virtualTimer().currRealTime() >= _lastShoot + _cooldown) {
		Vector2D shootPos = { pos.getX() + (_tr->getWidth() / 2), pos.getY() + (_tr->getHeight() / 2) };
		Vector2D shootDir = (target - shootPos).normalize();
		callback(shootPos, shootDir);
		_lastShoot = sdlutils().virtualTimer().currRealTime();
	}
}
