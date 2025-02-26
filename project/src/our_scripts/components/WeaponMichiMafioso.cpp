#include "WeaponMichiMafioso.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"

WeaponMichiMafioso::WeaponMichiMafioso() : Weapon(4, 5000, 20.0f, 0.1f, "sdl_logo") { }

WeaponMichiMafioso::~WeaponMichiMafioso() {}

void 
WeaponMichiMafioso::callback(Vector2D shootPos, Vector2D shootDir) {
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width;
	bp.sprite_key = "sdl_logo";
	bp.height = _attack_height;
	bp.rot = atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI;

	static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::BULLET);
}