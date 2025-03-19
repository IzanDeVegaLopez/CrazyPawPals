#include "Fog.h"

#include "../../ecs/Manager.h"
#include "../../game/Game.h"

#include "Transform.h"

void
Fog::initComponent() {
	//_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	//assert(_playerTr != nullptr);
	_fogTex = &sdlutils().images().at("fog").get_texture();

	int texWidth, texHeight;
	SDL_QueryTexture(_fogTex, nullptr, nullptr, &texWidth, &texHeight);

	fogRightEnd = sdlutils().width() / 2 + texWidth / 2;
	fogLeftEnd = sdlutils().width() / 2 - texWidth / 2;
	fogUpEnd = sdlutils().height() / 2 - texHeight / 2;
	fogDownEnd = sdlutils().height() / 2 + texHeight / 2;

	fogRightOrigin = sdlutils().width();
	fogLeftOrigin = -texWidth;
	fogUpOrigin = -texHeight;
	fogDownOrigin = sdlutils().height();

	fogRight = { /*x*/int(fogRightOrigin), /*y*/sdlutils().height() / 2 - texHeight / 2, /*w*/texHeight, /*h*/texWidth };
	fogLeft = { /*x*/int(fogLeftOrigin), /*y*/sdlutils().height() / 2 - texHeight / 2, /*w*/texHeight, /*h*/texWidth };
	fogUp = { /*x*/sdlutils().width() / 2 - texWidth / 2, /*y*/int(fogUpOrigin), /*w*/texWidth, /*h*/texHeight };
	fogDown = { /*x*/sdlutils().width() / 2 - texWidth / 2, /*y*/int(fogDownOrigin), /*w*/texWidth, /*h*/texHeight };
}

void
Fog::update(uint32_t delta_time) {
	(void)delta_time;

	if (!fogActive) {
		fogPercentage = 0;
	}
	else {
		if (fogPercentage < 1) {
			fogPercentage += 0.01;
		}

		fogRight.x = fogRightOrigin - (fogRightOrigin - fogRightEnd) * fogPercentage;
		fogLeft.x = fogLeftOrigin + (fogLeftEnd - fogLeftOrigin) * fogPercentage;
		fogUp.y = fogUpOrigin + (fogUpEnd - fogUpOrigin) * fogPercentage;
		fogDown.y = fogDownOrigin - (fogDownOrigin - fogDownEnd) * fogPercentage;

		SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogUp);
		SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogDown);
		SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogRight);
		SDL_RenderCopy(sdlutils().renderer(), _fogTex, nullptr, &fogLeft);

	}
}