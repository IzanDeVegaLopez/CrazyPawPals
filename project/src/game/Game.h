#pragma once
#include "../utils/Singleton.h"
#include "../utils/Vector2D.h"
#include "../ecs/ecs.h"
#include <vector>

namespace ecs {
class Manager;
}
class Scene;
class Game: public Singleton<Game>  {
public:
	enum State {
		MAINMENU,
		GAMESCENE,
		SELECTIONMENU,
		NUM_SCENE,
	};
	friend Singleton<Game>;
	virtual ~Game();
	bool init();
	void start();


	ecs::entity_t createSarnoRata(Vector2D);
	ecs::entity_t createPlimPlim(Vector2D);
	ecs::entity_t createBoom(Vector2D);
	ecs::entity_t createMichiMafioso(Vector2D);

	ecs::entity_t create_environment();

	ecs::Manager* get_mngr();
	Scene* get_currentScene();
	void change_Scene(State);
	std::pair<int,int> get_world_half_size() const;

private:
	int _current_scene_index = -1;
	std::vector<Scene*> _scenes;
	std::pair<int, int> _screen_size = std::make_pair(960,540);
	Game();
	ecs::Manager* _mngr;

	void create_scenes();
};
