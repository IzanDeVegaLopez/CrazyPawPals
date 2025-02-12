#pragma once
#include "Container.h"

class Player : public Container {
protected:
	float _shootCooldown;
	float _lastShoot;
	float _maxSpeed;

	std::vector<GameObject*>* b;
public:
	Player(std::vector<GameObject*>* b);
	virtual ~Player();
	void shoot(const Vector2D& target);
	void move();
};
