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
		std::string sprite_key;
	};
	struct ButtonProperties {
		Vector2D pos;
		float width = 40.0f;
		float height = 40.0f;
		float rot = 0.0f;
		std::string sprite_key;
	};
	enum WeaponType {
		REVOLVER,
		RAMPAGE,
		LAST_WEAPON,
	};
}