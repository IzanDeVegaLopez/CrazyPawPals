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
	Game::Instance()->get_gameScene()->generate_proyectile(bp, ecs::grp::BULLET);
}