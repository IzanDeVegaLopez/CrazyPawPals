#include "Health.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"

Health::Health(): _lives(DEFAULT_LIVES), _heartTexture(&sdlutils().images().at("heart")) {}

Health::~Health() {}

void 
Health::render() {
	int x = 10;
	int y = 10;
	int separation = _heartTexture->width();
	for (int i = 0; i < _lives; ++i) {
		_heartTexture->render(x + separation * i, y);
	}
}
void 
Health::release() {

}


