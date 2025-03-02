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
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/EnemyMovement.h"
#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/components/dyn_image.hpp"
#include "../our_scripts/components/camera_component.hpp"
#include "../our_scripts/components/rect_component.hpp"
#include "../our_scripts/components/Revolver.h"
#include "../our_scripts/components/Rampage.h"
//Scenes for SceneManager
#include "Scene.h"
#include "MainMenuScene.h"
#include "SelectionMenuScene.h"
#include "GameScene.h"


using namespace std;

Game::Game() : _mngr(nullptr){}

Game::~Game() {

	for (auto scene : _scenes) {
		if (scene != nullptr) {
			delete scene;
		}
	}
	_scenes.clear();

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();

}

bool Game::init() {
	
	// initialize the SDL singleton
	if (!SDLUtils::Init("crazy paw pals", _screen_size.first, _screen_size.second,
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

	// Inicializar el vector de escenas
	_scenes.resize(NUM_SCENE);

	//_scenes[MAINMENU] = new MainMenuScene();
	_scenes[GAMESCENE] = new GameScene();
	_scenes[GAMESCENE]->initScene();

	_scenes[SELECTIONMENU] = new SelectionMenuScene();
	_scenes[SELECTIONMENU]->initScene();

	_current_scene_index = SELECTIONMENU;
	return true;
}



void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();
	//delta time
	constexpr static const uint32_t target_delta_time_milliseconds = 10;

	uint64_t last_frame_start_tick = SDL_GetTicks64();
	SDL_Delay(target_delta_time_milliseconds);

	while (!exit) {
		const uint64_t frame_start_tick = SDL_GetTicks64();
		const uint32_t delta_time_milliseconds = frame_start_tick - last_frame_start_tick;
		assert(delta_time_milliseconds > 0 && "fatal error: delta time must be strictly positive");
		sdlutils().virtualTimer() = VirtualTimer{
			.current_time = frame_start_tick
		};

		last_frame_start_tick = frame_start_tick;
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}
		if (ihdlr.closeWindowEvent()) {
			exit = true;
			continue;
		}
		
		_scenes[_current_scene_index]->update(delta_time_milliseconds);
		_mngr->refresh();


		sdlutils().clearRenderer();
		_scenes[_current_scene_index]->render();
		sdlutils().presentRenderer();

		const uint64_t frame_end_tick = SDL_GetTicks64();
		const uint32_t frame_duration_milliseconds = frame_end_tick - frame_start_tick;
		if (frame_duration_milliseconds < target_delta_time_milliseconds) {
			SDL_Delay(target_delta_time_milliseconds - frame_duration_milliseconds);
		}
	}

}

ecs::Manager* Game::get_mngr() {
	return _mngr;
}

Scene* Game::get_currentScene() {
	return _scenes[_current_scene_index];
}

std::pair<int, int> Game::get_world_half_size() const
{
	return std::pair<int, int>(15,8);
}

void Game::change_Scene(State nextScene){
	if (nextScene < 0 || nextScene >= NUM_SCENE) {
		std::cerr << "Error: Invalid scene index" << std::endl;
		return;
	}

	if (_current_scene_index != -1) {
		_scenes[_current_scene_index]->exitScene();
	}
	_current_scene_index = nextScene;
	_scenes[_current_scene_index]->enterScene();
}
