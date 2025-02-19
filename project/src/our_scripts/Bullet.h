#pragma once
#include "../game/GameObject.h"

struct BulletProperties {
	Vector2D init_pos;
	Vector2D dir;
	int speed = 0;
	float life_time = 1.0f;
	float width = 40;
	float height = 40;
};

class Bullet
{
public:
	Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, Vector2D& prevDir, float speed, float lifeTime = 2.0f);
	virtual ~Bullet() {};
private:
	BulletProperties _properties;
	Transform* _tr;
};


