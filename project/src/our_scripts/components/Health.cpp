#include "Health.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/Font.h"
#include <iostream>

Health::Health(int maxHealth) 
	: _currentHealth(maxHealth), _maxHealth(maxHealth) {};
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
	_currentHealth -= damage;
	if (_currentHealth <= 0) {
		std::cout << "muerto" << std::endl;
		//muerto
	}
}

void
Health::setMaxHeatlh(int h) {
	if (h > 0) {
		_maxHealth = h;
	}
}