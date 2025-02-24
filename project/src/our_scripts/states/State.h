// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../game/Game.h"
#include "../our_scripts/components/Health.h"
#include "../our_scripts/components/Transform.h"
//#include "../our_scripts/components/ShootComponent.h"
#include "../components/EnemyStateMachine.h"
#include "../components/Weapon.h"

class State {
public:
	virtual ~State() {
	}
	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;
};

