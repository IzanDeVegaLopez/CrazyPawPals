#include "WeaponPlimPlim.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"

WeaponPlimPlim::WeaponPlimPlim() : Weapon(4, 5000, 20.0f, 0.1f, "p_plimplim", 2.0f, 2.0f) { }

WeaponPlimPlim::~WeaponPlimPlim() {}

void 
WeaponPlimPlim::callback(Vector2D shootPos, Vector2D shootDir) {
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width;
	bp.sprite_key = "p_plimplim";
	bp.height = _attack_height;
	auto* scene = static_cast<GameScene*>(Game::Instance()->get_currentScene());

	float initialRot = atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI;
	bp.rot = initialRot;

	//Dispara 3 balas
	for (int i = 0; i < 3; ++i) {
		float angleOffset = i * 6.0f * (M_PI / 180.0f); // Convertir a radianes
		Vector2D rotatedDir(
			shootDir.getX() * cos(angleOffset) - shootDir.getY() * sin(angleOffset),
			shootDir.getX() * sin(angleOffset) + shootDir.getY() * cos(angleOffset)
		);
		bp.dir = rotatedDir;
		bp.rot = atan2(rotatedDir.getY(), rotatedDir.getX()) * 180.0f / M_PI;

		scene->generate_proyectile(bp, ecs::grp::BULLET);
	}
}