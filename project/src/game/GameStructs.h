#pragma once
#include "../utils/Vector2D.h"

namespace GameStructs {
	struct BulletProperties {
		Vector2D init_pos;
		Vector2D dir;
		int speed = 0;
		float life_time = 1.0f;
		float width = 40;
		float height = 40;
	};
}