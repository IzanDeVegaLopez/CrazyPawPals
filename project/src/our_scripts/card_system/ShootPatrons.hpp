#ifndef SHOOT_PATRONS
#define SHOOT_PATRONS

#include <algorithm>
#include "../../game/GameStructs.h"
#include "../../ecs/ecs.h"
#include "../../game/Game.h"
#include "../../game/scenes/GameScene.h"
//#include "../../utils/Vector2D.h

namespace patrons {

	inline void ShotgunPatron(GameStructs::BulletProperties& bp, ecs::grpId_t gId, int angle, int proyectile_number) {

		Vector2D initialRot = bp.dir;

		// Dispara 5 balas con �ngulos de -60, -30, 0, 30 y 60 grados
		for (int i = 0; i < proyectile_number; ++i) {
			//std::cout << "angle = " << (angle * (i / ((float)proyectile_number - 1)) - angle / 2.0f);
			float angleOffset = (angle * (i / (std::max(((float)proyectile_number - 1.0f),1.0f))) - angle / 2.0f) * (M_PI / 180.0f);//i * 6.0f * (M_PI / 180.0f); // Convertir a radianes
			Vector2D rotatedDir(
				initialRot.getX() * cos(angleOffset) - initialRot.getY() * sin(angleOffset),
				initialRot.getX() * sin(angleOffset) + initialRot.getY() * cos(angleOffset)
			);
			bp.dir = rotatedDir;
			//bp.rot = atan2(rotatedDir.getY(), rotatedDir.getX()) * 180.0f / M_PI;

			static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, gId);
		}
	}
}
#endif // !SHOOTPATRONS