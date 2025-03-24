#include "Fog.h"

#include "../../ecs/Manager.h"
#include "../../game/Game.h"

#include "movement/Transform.h"
#include "../../our_scripts/components/rendering/dyn_image.hpp"
#include "../../our_scripts/components/rendering/rect_component.hpp"


void
Fog::initComponent() {
	//auto _ent = Game::Instance()->get_mngr()->getHandler(ecs::hdlr::FOGGROUP); // esto es necesario ??
	fogTransform = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	fogImage = Game::Instance()->get_mngr()->getComponent<dyn_image>(_ent);
	fogRect = Game::Instance()->get_mngr()->getComponent<rect_component>(_ent);
}

void
Fog::update(uint32_t delta_time) {
	(void)delta_time;

	if (!fogActive) {
		fogPercentage = 0;
	}
	else {
		if (fogPercentage <= 1 - 0.001) {
			fogPercentage += 0.001;
		}
		std::cout << "Fog: " << fogPercentage * 100 << "%" << std::endl;
		float targetX = 0.0f;
		float targetY = 0.0f;

		float currentX = fogTransform->getPos().getX();
		float currentY = fogTransform->getPos().getY();

		float newX = currentX + (targetX - currentX) * fogPercentage;
		float newY = currentY + (targetY - currentY) * fogPercentage;

		fogTransform->setPos(Vector2D(newX, newY));
	}
}