// This file is part of the course TPV2@UCM - Samir Genaim
#include "../ecs/ecs.h"

#pragma once

namespace ecs {
class Manager;
}

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();

	ecs::entity_t createSarnoRata();
	ecs::entity_t createPlimPlim();
	ecs::entity_t createBoom();
	ecs::entity_t createMichiMafioso();
private:
	void checkCollisions();
	ecs::Manager *_mngr;
};

