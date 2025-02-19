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
	ecs::Manager* get_mngr();
private:
	Game();
	void checkCollisions();
	ecs::Manager *_mngr;
};

