#include "Health.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/Font.h"
#include <iostream>

Health::Health(int maxHealth) 
	: _currentHealth(maxHealth), _maxHealth(maxHealth),_shield(0) {};
Health::~Health() {};

void
Health::heal(int health) {
	std::cout << "curar" << std::endl;
	if (_currentHealth + health < _maxHealth) {
		_currentHealth += health;
	}
	else _currentHealth = _maxHealth;
}

void
Health::takeDamage(int damage) {
	std::cout << "da" << std::endl;
	if (_shield <= 0) {
		_currentHealth -= damage;
		if (_currentHealth <= 0) {
			std::cout << "muerto" << std::endl;
			//muerto
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
Health::getHealth() { return _currentHealth; }
void 
Health::takeShield(int s) {
	_shield = s;
}
void
Health::update(uint32_t delta_time) {
	(void)delta_time;
	if (_shield > 0 && sdlutils().virtualTimer().currRealTime() >= _shieldTime) {
		_shieldTime = sdlutils().virtualTimer().currRealTime() + FRAME_DURATION;
		_shield--;
	}
}