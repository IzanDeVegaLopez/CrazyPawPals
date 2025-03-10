#include "Fog.h"

#include "../../ecs/Manager.h"
#include "../../game/Game.h"

#include "Transform.h"

void
Fog::initComponent() {
	_playerTr = Game::Instance()->get_mngr()->getComponent<Transform>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::PLAYER)[0]);
	assert(_playerTr != nullptr);
	_fogTex = &sdlutils().images().at("fog");

	fogRightEnd = sdlutils().width() / 2 + _fogTex->width() / 2;
	fogLeftEnd = sdlutils().width() / 2 - _fogTex->width() / 2;
	fogUpEnd = sdlutils().height() / 2 - _fogTex->height() / 2;
	fogDownEnd = sdlutils().height() / 2 + _fogTex->height() / 2;

	fogRightOrigin = sdlutils().width();
	fogLeftOrigin = -_fogTex->width();
	fogUpOrigin = -_fogTex->height();
	fogDownOrigin = sdlutils().height();

	fogRight = { /*x*/fogRightOrigin, /*y*/sdlutils().height() / 2 - _fogTex->height() / 2, /*w*/_fogTex->height(), /*h*/_fogTex->width() };
	fogLeft = { /*x*/fogLeftOrigin, /*y*/sdlutils().height() / 2 - _fogTex->height() / 2, /*w*/_fogTex->height(), /*h*/_fogTex->width() };
	fogUp = { /*x*/sdlutils().width() / 2 - _fogTex->width() / 2, /*y*/fogUpOrigin, /*w*/_fogTex->width(), /*h*/_fogTex->height() };
	fogDown = { /*x*/sdlutils().width() / 2 - _fogTex->width() / 2, /*y*/fogDownOrigin, /*w*/_fogTex->width(), /*h*/_fogTex->height() };
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

		SDL_RenderCopy(sdlutils().renderer(), _fogTex->getTexture(), nullptr, &fogUp);
		SDL_RenderCopy(sdlutils().renderer(), _fogTex->getTexture(), nullptr, &fogDown);
		SDL_RenderCopy(sdlutils().renderer(), _fogTex->getTexture(), nullptr, &fogRight);
		SDL_RenderCopy(sdlutils().renderer(), _fogTex->getTexture(), nullptr, &fogLeft);

	}
}