#pragma once
#include "../game/GameObject.h"
#include "../utils/Vector2D.h"

class Bullet
{
public:
	struct BulletProperties {
		Vector2D init_pos;
		Vector2D dir;
		int speed = 0;
		float life_time = 1.0f;
		float width = 40;
		float height = 40;
	};
	static void generate_proyectile(BulletProperties&, ecs::grpId_t gId = ecs::grp::PLAYERBULLETS);
};


