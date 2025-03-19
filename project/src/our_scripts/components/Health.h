#pragma once
#include "../../ecs/Component.h"

class Health :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::HEALTH);

	Health(int maxHealth);
	virtual ~Health();
	void takeDamage(int damage);
	void heal(int health);
	int getMaxHealth() const;
	void setMaxHeatlh(int h);
	int getHealth() const;
	void takeShield(int s);
	void payHealth(int cost);
	void update(uint32_t delta_time) override;

protected:
	int _currentHealth;
	int _maxHealth;
	int _shield;
	uint32_t _shieldTime;

	const int FRAME_DURATION = 1000;
};
