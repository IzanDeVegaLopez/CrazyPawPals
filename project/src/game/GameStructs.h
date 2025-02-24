#pragma once
#include "../utils/Vector2D.h"

namespace GameStructs {
	struct BulletProperties {
		Vector2D init_pos;
		Vector2D dir;
		float speed = 0.0f;
		int damage = 0;
		float life_time = 1.0f;
		float width = 40;
		float height = 40;
		float rot = 0.0f;
	};
}