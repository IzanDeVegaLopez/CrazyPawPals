// This file is part of the course TPV2@UCM - Samir Genaim
#include "../ecs/ecs.h"

#pragma once
#include "../utils/Singleton.h";
namespace ecs {
class Manager;
}

class Game : public Singleton<Game> {
public:
	friend Singleton<Game>;
	virtual ~Game();
	bool init();
	void start();


	ecs::entity_t createSarnoRata();
	ecs::entity_t createPlimPlim();
	ecs::entity_t createBoom();
	ecs::entity_t createMichiMafioso();

	ecs::Manager* get_mngr();

private:
	Game();
	void checkCollisions();
	ecs::Manager *_mngr;
};

