#include "WeaponBoom.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"

WeaponBoom::WeaponBoom() : Weapon(4, 5000, 20.0f, 0.0f, "sdl_logo", 1.0f, 1.0f) { }

WeaponBoom::~WeaponBoom() {}

void 
WeaponBoom::callback(Vector2D shootPos, Vector2D shootDir) {
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	float dist = 0.1;
	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width;
	bp.sprite_key = "sdl_logo";
	bp.height = _attack_height;
	bp.rot = atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI;
	auto* scene = static_cast<GameScene*>(Game::Instance()->get_currentScene());
	float alpha = 90 * (180.0f / M_PI);
	int n = 9;

	float delta =  (360 / n);
	for (int i = 1; i < n;++i) {
		scene->generate_proyectile(bp, ecs::grp::BULLET);
		bp.init_pos = { bp.init_pos.getX() + cos(alpha), bp.init_pos.getY() - sin(alpha) };
		alpha += delta;
	}
}