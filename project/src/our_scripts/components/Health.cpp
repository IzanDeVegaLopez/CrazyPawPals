#include "Health.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/Font.h"
#include "../../ecs/Manager.h"
#include "../../game/Game.h"
#include <algorithm>

Health::Health(int maxHealth, bool isPlayer) 
	: _is_player(isPlayer), _currentHealth(maxHealth), _maxHealth(maxHealth), _shield(0), _shieldTime(0) {};
Health::~Health() {};

void
Health::heal(int health) {
	if (_currentHealth + health < _maxHealth) _currentHealth += health;
	else _currentHealth = _maxHealth;
}
int Health::getMaxHealth() const { return _maxHealth; }

void
Health::takeDamage(int damage) {
	if (_shield <= damage) {
		damage -= _shield;
		_shield = 0;

		_currentHealth -= damage;

		if (_currentHealth <= 0) {
			if (_is_player) {
				Game::Instance()->get_event_mngr()->fire_event(event_system::player_dead, event_system::event_receiver::Msg());
			}
			else {
				event_system::event_receiver::Msg msg;
				msg.int_value = _maxHealth * 0.2;
				Game::Instance()->get_event_mngr()->fire_event(event_system::enemy_dead, msg);
			}

			Game::Instance()->get_mngr()->setAlive(_ent, false);
		}
	}
	else _shield -= damage;
}

void
Health::setMaxHeatlh(int maxHeatlh) {
	if (maxHeatlh > 0) _maxHealth = maxHeatlh;
}
int
Health::getHealth() const { return _currentHealth; }
void 
Health::takeShield(int shield) {
	_shield = shield;
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