#include "PumpShotgun.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"


PumpShotgun::PumpShotgun() : Weapon(1, 0.9f, 1.5f, 2.0f, "pacman"), _has_mill(false) {}

PumpShotgun::~PumpShotgun() {}

void
PumpShotgun::callback(Vector2D shootPos, Vector2D shootDir) {
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();

	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed = _speed;
	bp.damage = _damage;
	bp.life_time = _lifeTime;
	bp.width = _attack_width;
	bp.height = _attack_height;

	int numBullets = _has_mill ? 8 : 4;
	float totalAngle = 60.0f;

	float initialRot = atan2(bp.dir.getY(), bp.dir.getX()) - (totalAngle / 2.0f);

	float angleIncrement = totalAngle / (numBullets - 1);

	auto* scene = static_cast<GameScene*>(Game::Instance()->get_currentScene());

	for (int i = 0; i < numBullets; ++i) {
		float angleOffset = (initialRot + (i * angleIncrement)) * (M_PI / 180.0f); // Convertir a radianes
		Vector2D rotatedDir(
			shootDir.getX() * cos(angleOffset) - shootDir.getY() * sin(angleOffset),
			shootDir.getX() * sin(angleOffset) + shootDir.getY() * cos(angleOffset)
		);
		bp.dir = rotatedDir;
		bp.rot = atan2(rotatedDir.getY(), rotatedDir.getX()) * 180.0f / M_PI;

		scene->generate_proyectile(bp, ecs::grp::BULLET, _tex);
	}
	if (_has_mill) _has_mill = false;
}