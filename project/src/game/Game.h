#pragma once
#include "../utils/Singleton.h"
#include "../utils/Vector2D.h"
#include "../ecs/ecs.h"

namespace ecs {
class Manager;
}
class Scene;
class GameScene;
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

	ecs::Manager* get_mngr();
	Scene* get_currentScene();
	std::pair<int,int> get_world_half_size() const;

private:
	void change_Scene(State);
	Scene* _current_scene;
	Scene* _game_scene;
	Scene* _mainmenu_scene;
	Scene* _selectionmenu_scene;
	std::pair<int, int> _screen_size = std::make_pair(800,600);
	Game();
	ecs::Manager* _mngr;
};