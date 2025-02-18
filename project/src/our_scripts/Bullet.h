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
	//Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, float speed, float lifeTime = 2.0f);
	virtual ~Bullet();
	static void generate_proyectile(BulletProperties&, ecs::grpId_t gId = ecs::grp::PLAYERBULLETS);
	static Vector2D get_direction(Vector2D& startPos, Vector2D& targetpos);
private:
	BulletProperties _properties;
	Transform* _tr;
};


