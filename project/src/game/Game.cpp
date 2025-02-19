// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/Transform.h"
#include "../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../our_scripts/components/MovementController.h"
#include "../our_scripts//components/ShootComponent.h"
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/Deck.hpp"

//Scenes for SceneManager
#include "Scene.h"
#include "MainMenuScene.h"
#include "SelectionMenuScene.h"
#include "GameScene.h"

using namespace std;

Game::Game() : _mngr(nullptr){}

Game::~Game() {

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {

	// initialize the SDL singleton
	if (!SDLUtils::Init("crazy paw pals", 800, 600,
		"resources/config/crazypawpals.resources.json")) {

		std::cerr << "Something went wrong while initializing SDLUtils"
				<< std::endl;
		return false;

	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return false;
	}

	// enable the cursor visibility
	SDL_ShowCursor(SDL_ENABLE);

	_mngr = new ecs::Manager();

	_game_scene = new GameScene();
	_game_scene->initScene();
	_current_scene = _game_scene;

}

ecs::Manager* Game::get_mngr() {
	return _mngr;
}
GameScene* Game::get_gameScene() {
	return static_cast<GameScene*>(_game_scene);
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();
	//delta time
	Uint32 dt = 10;

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		// store the current time -- all game objects should use this time when
		// then need to the current time. They also have accessed to the time elapsed
		// between the last two calls to regCurrTime().
		Uint32 startTime = sdlutils().regCurrTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}
		if (ihdlr.closeWindowEvent()) {
			exit = true;
			continue;
		}

		_current_scene->update(dt);
		_mngr->refresh();


		sdlutils().clearRenderer();
		_current_scene->render();
		sdlutils().presentRenderer();

		dt = sdlutils().currRealTime() - startTime;
		if (dt < 10)
			SDL_Delay(10 - dt);
	}


}

void Game::change_Scene(State nextScene){
	switch (nextScene) {
	case Game::MAINMENU:
		break;	
	case Game::GAMESCENE:
		break;
	case Game::SELECTIONMENU:
		break;
	}

}


