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

	void setMaxHeatlh(int h);

protected:
	int _currentHealth;
	int _maxHealth;
};
