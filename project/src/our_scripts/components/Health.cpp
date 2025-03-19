#include "Health.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/Font.h"
#include "../../ecs/Manager.h"
#include "../../game/Game.h"
#include <algorithm>

Health::Health(int maxHealth) 
	: _currentHealth(maxHealth), _maxHealth(maxHealth),_shield(0),_shieldTime(0) {};
Health::~Health() {};

void
Health::heal(int health) {
	if (_currentHealth + health < _maxHealth) _currentHealth += health;
	else _currentHealth = _maxHealth;
}
int Health::getMaxHealth() const { return _maxHealth; }

void
Health::takeDamage(int damage) {
	if (_shield <= 0) {
		_currentHealth -= damage;
		if (_currentHealth <= 0) {
			Game::Instance()->get_mngr()->setAlive(_ent, false);
		}
	}
	else _shield -= damage;

}

void
Health::setMaxHeatlh(int h) {
	if (h > 0) {
		_maxHealth = h; 
	}
}
int
Health::getHealth() const { return _currentHealth; }
void 
Health::takeShield(int s) {
	_shield = s;
}
void Health::payHealth(int cost)
{
	_currentHealth = std::max(1, _currentHealth - cost);
}
void
Health::update(uint32_t delta_time) {
	(void)delta_time;
	if (_shield > 0 && sdlutils().virtualTimer().currRealTime() >= _shieldTime) {
		_shieldTime = sdlutils().virtualTimer().currRealTime() + FRAME_DURATION;
		_shield--;
	}
}