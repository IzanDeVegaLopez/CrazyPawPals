#pragma once
#include "../utils/Vector2D.h"
#include "../ecs/ecs.h"
#include "../rendering/rect.hpp"

namespace GameStructs {
	enum WeaponType {
		REVOLVER,
		RAMPAGE,
		PUMP_SHOTGUN,
		RAMP_CANON,
		LIGHTBRINGER,
		LAST_WEAPON,
	};
	enum DeckType {
		ONE,
		TWO,
		THREE,
		FOUR,
		LAST_DECK,
	};
	struct BulletProperties {
		Vector2D init_pos;
		Vector2D dir;
		float speed = 0.0f;
		int damage = 0;
		float life_time = 1.0f;
		float width = 40;
		float height = 40;
		std::string sprite_key;
		WeaponType weapon_type;
	};
	struct ButtonProperties {
		rect_f32 rect;
		float rot = 0.0f;
		std::string sprite_key;
		ecs::grpId_t ID;
	};
}