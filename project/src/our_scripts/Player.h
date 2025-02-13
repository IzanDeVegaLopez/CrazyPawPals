#pragma once
#include "../game/GameObject.h"


class Player : public GameObject {
protected:
	float _shootCooldown;
	float _maxSpeed;
	Vector2D _prevDir;
	Transform* _tr;

public:
	Player(ecs::Manager* mngr);
	virtual ~Player() {};
	//void shoot(const Vector2D& target);
	//void move();
};




