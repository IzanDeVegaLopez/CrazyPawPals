#include "Rampage.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"


Rampage::Rampage() : Weapon(5, 0.55f, 15.0f, 2.0f, "rampage") { }

Rampage::~Rampage() {}

void 
Rampage::callback(Vector2D shootPos, Vector2D shootDir) {
	//float initialRot = atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI;
	for (int i = 1; i <= 2; ++i) {
		GameStructs::BulletProperties bp = GameStructs::BulletProperties();
		bp.dir = shootDir;
		bp.init_pos = shootPos;
		bp.speed = _speed;
		bp.damage = _damage;
		bp.life_time = 2;
		bp.width = _attack_width;
		bp.height = _attack_height;
		//bp.rot = rot * i;
		static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::BULLET, _tex);
	}
}