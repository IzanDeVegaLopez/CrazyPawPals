// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

namespace ecs {
class Manager;
}
class Game {
public:
	enum State{
		MAINMENU,
		GAMESCENE,
		SELECTIONMENU,
		NUM_SCENE,
	};

	Game();
	virtual ~Game();
	void init();
	void start();
private:
	void change_Scene(State);
	Scene* _current_scene;
	Scene* _game_scene;
	Scene* _mainmenu_scene;
	Scene* _selectionmenu_scene;
};

