#pragma once
#include "../utils/Singleton.h";

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
	ecs::Manager* get_mngr();
	GameScene* get_gameScene();
private:
	void change_Scene(State);
	Scene* _current_scene;
	Scene* _game_scene;
	Scene* _mainmenu_scene;
	Scene* _selectionmenu_scene;
	Game();
	ecs::Manager* _mngr;
};

