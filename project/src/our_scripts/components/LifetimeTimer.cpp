
#include "LifetimeTimer.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Manager.h"
#include "../../game/Game.h"
LifetimeTimer::LifetimeTimer(float lifetime)
	:_lifetime(sdlutils().virtualTimer().currRealTime() + lifetime * 1000) {}

LifetimeTimer::~LifetimeTimer() {
}
void LifetimeTimer::update(Uint32 delta_time) {
	if (sdlutils().virtualTimer().currRealTime() > _lifetime) {
		Game::Instance()->get_mngr()->setAlive(_ent, false);
	}
}
