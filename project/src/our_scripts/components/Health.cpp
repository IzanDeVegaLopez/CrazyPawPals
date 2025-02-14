#include "Health.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/Font.h"

Health::Health() : _lives(DEFAULT_LIVES), _tex(&sdlutils().images().at("heart")) {}

Health::~Health() {}

void
Health::render() {
	SDL_Rect dest = build_sdlrect(10, 10, _tex->width()/4, _tex->height()/4);

	assert(_tex != nullptr);
	_tex->render(dest);

	int separation = 10 + _tex->width() / 4 + 10;
	auto& healthText = sdlutils().msgs().at("life");
	healthText.render(separation, 10);
}

void
Health::update() {

}



