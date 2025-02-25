#pragma once
#include "../../game/GameStructs.h"
#include "../../ecs/ecs.h"
#include "../../game/Game.h"
//#include "../../utils/Vector2D.h
void ShotgunPatron(GameStructs::BulletProperties& bp, ecs::grpId_t gId, int angle, int proyectile_number) {

	Vector2D initialRot = bp.dir;

	// Dispara 5 balas con ángulos de -60, -30, 0, 30 y 60 grados
	for (int i = 0; i < proyectile_number; ++i) {
		float angleOffset = (angle * (i/(float)proyectile_number) - angle/2.0f) * (M_PI/180.0f);//i * 6.0f * (M_PI / 180.0f); // Convertir a radianes
		Vector2D rotatedDir(
			initialRot.getX() * cos(angleOffset) - initialRot.getY() * sin(angleOffset),
			initialRot.getX() * sin(angleOffset) + initialRot.getY() * cos(angleOffset)
		);
		bp.dir = rotatedDir;
		bp.rot = atan2(rotatedDir.getY(), rotatedDir.getX()) * 180.0f / M_PI;

		static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp,gId);
	}
}