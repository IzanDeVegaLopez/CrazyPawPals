#include "Rampage.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"
#include "../card_system/ShootPatrons.hpp"


Rampage::Rampage() : Weapon(5, 0.55f, 1.5f, 2.0f, "rampage") { }

Rampage::~Rampage() {}

void 
Rampage::callback(Vector2D shootPos, Vector2D shootDir) {
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();

	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed = _speed;
	bp.damage = _damage;
	bp.life_time = _distance;
	bp.width = _attack_width;
	bp.height = _attack_height;
	bp.sprite_key = _tex;

	ShotgunPatron(bp, ecs::grp::BULLET, 180, 5);

	//float initialRot = atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI;
	//bp.rot = initialRot;
	/*
	auto* scene = static_cast<GameScene*>(Game::Instance()->get_currentScene());

	// Dispara 5 balas con ángulos de -60, -30, 0, 30 y 60 grados
	for (int i = -2; i <= 2; ++i) {
		float angleOffset = i * 6.0f * (M_PI / 180.0f); // Convertir a radianes
		Vector2D rotatedDir(
			shootDir.getX() * cos(angleOffset) - shootDir.getY() * sin(angleOffset),
			shootDir.getX() * sin(angleOffset) + shootDir.getY() * cos(angleOffset)
		);
		bp.dir = rotatedDir;
		bp.rot = atan2(rotatedDir.getY(), rotatedDir.getX()) * 180.0f / M_PI;

		scene->generate_proyectile(bp, ecs::grp::BULLET);
	}*/
}