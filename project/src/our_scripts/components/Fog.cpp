#include "Fog.h"

#include "../../ecs/Manager.h"
#include "../../game/Game.h"

#include "movement/Transform.h"

void
Fog::initComponent() {
	//_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	//assert(_playerTr != nullptr);
	_fogTex = &sdlutils().images().at("fog").get_texture();

	SDL_QueryTexture(_fogTex, nullptr, nullptr, &texWidth, &texHeight);
	worldWidth = Game::Instance()->get_world_half_size().first * 2;
	worldHeight = Game::Instance()->get_world_half_size().second * 2;

	fogRightEnd = worldWidth / 2 + texWidth / 2;
	fogLeftEnd = worldWidth / 2 - texWidth / 2;
	fogUpEnd = worldHeight / 2 - texHeight / 2;
	fogDownEnd = worldHeight / 2 + texHeight / 2;

	fogRightOrigin = worldWidth;
	fogLeftOrigin = -texWidth;
	fogUpOrigin = -texHeight;
	fogDownOrigin = worldHeight;

	fogRight = { /*x*/int(fogRightOrigin), /*y*/worldHeight / 2 - texHeight / 2, /*w*/texHeight, /*h*/texWidth };
	fogLeft = { /*x*/int(fogLeftOrigin), /*y*/worldHeight / 2 - texHeight / 2, /*w*/texHeight, /*h*/texWidth };
	fogUp = { /*x*/worldWidth / 2 - texWidth / 2, /*y*/int(fogUpOrigin), /*w*/texWidth, /*h*/texHeight };
	fogDown = { /*x*/worldWidth / 2 - texWidth / 2, /*y*/int(fogDownOrigin), /*w*/texWidth, /*h*/texHeight };
}

void
Fog::update(uint32_t delta_time) {
	(void)delta_time;

	if (!fogActive) {
		fogPercentage = 0;
	}
	else {
		if (fogPercentage < 1) {
			fogPercentage += 0.001;
		}
		std::cout << "Fog: " << fogPercentage/* * 100*/ << "%" << std::endl;
		//fogRight.x = fogRightOrigin - (fogRightOrigin - fogRightEnd) * fogPercentage;
		//fogLeft.x = fogLeftOrigin + (fogLeftEnd - fogLeftOrigin) * fogPercentage;
		//fogUp.y = fogUpOrigin + (fogUpEnd - fogUpOrigin) * fogPercentage;
		//fogDown.y = fogDownOrigin - (fogDownOrigin - fogDownEnd) * fogPercentage;

		//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogUp);
		//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogDown);
		//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogRight);
		//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogLeft);
		fogUp = { 0, 0, worldWidth, worldHeight};

		SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogUp);

	}
}