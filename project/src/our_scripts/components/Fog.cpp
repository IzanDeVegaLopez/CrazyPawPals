#include "Fog.h"

#include "../../ecs/Manager.h"
#include "../../game/Game.h"

#include "movement/Transform.h"
#include "../../our_scripts/components/rendering/dyn_image.hpp"
#include "../../our_scripts/components/rendering/rect_component.hpp"


void
Fog::initComponent() {
	fogTransform = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	fogImage = Game::Instance()->get_mngr()->getComponent<dyn_image>(_ent);
	fogRect = Game::Instance()->get_mngr()->getComponent<rect_component>(_ent);
	orSize = fogRect->getSize();
	orW = fogTransform->getWidth();
	orH = fogTransform->getHeight();
}

void Fog::update(uint32_t delta_time) {
	(void)delta_time;

	if (!fogActive) {
		fogPercentage = 0;
	}
	else {
		if (fogPercentage <= 1 - 0.0005) {
			fogPercentage += 0.0005;
		}
		std::cout << "Fog: " << fogPercentage * 100 << "%" << std::endl;

		fogRect->setSize(orSize * (1 - fogPercentage));
	}
}