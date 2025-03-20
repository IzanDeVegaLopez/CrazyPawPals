#include "Weapon.h"
#include "../../../game/Game.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../sdlutils/VirtualTimer.h"
#include "../movement/Transform.h"

//Weapon::Weapon() : _damage(0), _cooldown(0.0f), _distance(0.0f), _speed(0.0f), _tex(""), _attack_width(50), _attack_height(50) {}

Weapon::Weapon(int damage, float cd_ms, float distance, float speed, const std::string& texture_key, float w, float h)
	: _damage(damage), _cooldown(cd_ms), _distance(distance), _speed(speed), _tex(texture_key),
	_attack_width(w), _attack_height(h), _lastShoot(0.0f), _tr(nullptr) 
{
}

void 
Weapon::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
Weapon::~Weapon() {}

void 
Weapon::set_attack_size(float w, float h) {
	_attack_height = h; 
	_attack_width = w;
}
bool
Weapon::shoot(const Vector2D& target) {
	auto& pos = _tr->getPos();
	if (sdlutils().virtualTimer().currTime() >= _lastShoot + _cooldown) {
		Vector2D shootPos = { pos.getX(), pos.getY() };
		Vector2D shootDir = (target - shootPos).normalize();
		callback(shootPos, shootDir);
		_lastShoot = sdlutils().virtualTimer().currTime();
		return true;
	}return false;
}
