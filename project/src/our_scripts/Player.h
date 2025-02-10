#pragma once
#include "Container.h"
class Player : public Container {
protected:
	float _shootCooldown;
	float _lastShoot;
public:
	Player();
	virtual ~Player();
	void shoot(const Vector2D& target);
};
