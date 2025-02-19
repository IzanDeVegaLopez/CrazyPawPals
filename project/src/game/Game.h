// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include "../utils/Vector2D.h"
#include "../ecs/ecs.h"
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


	ecs::entity_t createSarnoRata(Vector2D);
	ecs::entity_t createPlimPlim(Vector2D);
	ecs::entity_t createBoom(Vector2D);
	ecs::entity_t createMichiMafioso(Vector2D);

	ecs::Manager* get_mngr();

private:
	Game();
	void checkCollisions();
	ecs::Manager *_mngr;
};

