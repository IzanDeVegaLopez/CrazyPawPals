// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../utils/Singleton.h"

namespace ecs {
class Manager;
}

class Game : public Singleton<Game> {
public:
	friend Singleton<Game>;
	virtual ~Game();
	bool init();
	void start();
	ecs::Manager* get_mngr();
	std::pair<int, int> get_screen_size() const;
private:
	std::pair<int, int> _screen_size = std::make_pair(800,600);
	Game();
	void checkCollisions();
	ecs::Manager *_mngr;
};

