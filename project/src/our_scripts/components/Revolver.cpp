#include "Revolver.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"

Revolver::Revolver() : Weapon() { }

Revolver::~Revolver() {}

void 
Revolver::callback(Vector2D shootPos, Vector2D shootDir) {
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width;
	bp.height = _attack_height;
	bp.rot = atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI;
	Game::Instance()->get_gameScene()->generate_proyectile(bp, ecs::grp::BULLET, _tex);
}