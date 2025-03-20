#include "Fog.h"

#include "../../ecs/Manager.h"
#include "../../game/Game.h"

#include "movement/Transform.h"
#include "../../our_scripts/components/rendering/transformless_dyn_image.h"

Fog::Fog(int nFog, transformless_dyn_image* tex)
{
	fogNum = nFog;
	_fogTex = tex->texture;
}

void
Fog::initComponent() {
	//_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	//assert(_playerTr != nullptr);

	//_fogTex = &sdlutils().images().at("fog").get_texture();
	//SDL_QueryTexture(_fogTex, nullptr, nullptr, &texWidth, &texHeight);
	
	texWidth = _fogTex->width();
	texHeight = _fogTex->height();
	
	worldWidth = Game::Instance()->get_world_half_size().first * 2;
	worldHeight = Game::Instance()->get_world_half_size().second * 2;

	switch (fogNum)
	{
	case 0:
		fogRightEnd = worldWidth / 2 + texWidth / 2;
		fogRightOrigin = worldWidth;
		fogRight = { /*x*/int(fogRightOrigin), /*y*/worldHeight / 2 - texHeight / 2, /*w*/texHeight, /*h*/texWidth };
		break;
	case 1:
		fogLeftEnd = worldWidth / 2 - texWidth / 2;
		fogLeftOrigin = -texWidth;
		fogLeft = { /*x*/int(fogLeftOrigin), /*y*/worldHeight / 2 - texHeight / 2, /*w*/texHeight, /*h*/texWidth };
		break;
	case 2:
		fogUpEnd = worldHeight / 2 - texHeight / 2;
		fogUpOrigin = -texHeight;
		fogUp = { /*x*/worldWidth / 2 - texWidth / 2, /*y*/int(fogUpOrigin), /*w*/texWidth, /*h*/texHeight };
		break;
	case 3:
		fogDownEnd = worldHeight / 2 + texHeight / 2;
		fogDownOrigin = worldHeight;
		fogDown = { /*x*/worldWidth / 2 - texWidth / 2, /*y*/int(fogDownOrigin), /*w*/texWidth, /*h*/texHeight };
		break;
	default:
		break;
	}
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

		switch (fogNum)
		{
		case 0:
			fogRight.x = fogRightOrigin - (fogRightOrigin - fogRightEnd) * fogPercentage;
			//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogRight);
			break;
		case 1:
			fogLeft.x = fogLeftOrigin + (fogLeftEnd - fogLeftOrigin) * fogPercentage;
			//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogLeft);
			break;
		case 2:
			fogUp.y = fogUpOrigin + (fogUpEnd - fogUpOrigin) * fogPercentage;
			//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogUp);
			break;
		case 3:
			fogDown.y = fogDownOrigin - (fogDownOrigin - fogDownEnd) * fogPercentage;
			//SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogDown);
			break;
		default:
			break;
		}

	}
}