
#include "LifetimeTimer.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Manager.h"
#include <iostream>
LifetimeTimer::LifetimeTimer(float lifetime)
	:_lifetime(sdlutils().currRealTime() + lifetime * 1000) {}

LifetimeTimer::~LifetimeTimer() {
}
void LifetimeTimer::update(int delta_time) {

	if (sdlutils().currRealTime() > _lifetime) {
		_ent->getMngr()->setAlive(_ent, false);
	}
}
