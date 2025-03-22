#include "WeaponSarnoRata.h"
#include "../../game/Game.h"
#include "../../game/scenes/GameScene.h"

WeaponSarnoRata::WeaponSarnoRata() : Weapon(4, 2000, 20.0f, 0.0f, "p_sarno_rata", 2.0f, 2.0f) { }

WeaponSarnoRata::~WeaponSarnoRata() {}

void 
WeaponSarnoRata::callback(Vector2D shootPos, Vector2D shootDir) {
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width;
	bp.height = _attack_height;
	bp.sprite_key = "p_sarno_rata";


	static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::BULLET);
}