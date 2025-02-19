
#include "LifetimeTimer.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Manager.h"
#include "../../game/Game.h"
#include <iostream>
LifetimeTimer::LifetimeTimer(float lifetime)
	:_lifetime(sdlutils().currRealTime() + lifetime * 1000) {}

LifetimeTimer::~LifetimeTimer() {
}
void LifetimeTimer::update(Uint32 delta_time) {
	//TODO: cambiar esto para que vaya con el timer virtual del profe para que cuando se pause el juego no explote
	//std::cout << sdlutils().currRealTime() << " > " << _lifetime << std::endl;
	if (sdlutils().currRealTime() > _lifetime) {
		Game::Instance()->get_mngr()->setAlive(_ent, false);
	}
}
