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
#include "../our_scripts/components/Revolver.h"

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

ecs::entity_t create_test_player_at(Vector2D position) {
	auto &&manager = *Game::Instance()->get_mngr();
	auto player = manager.addEntity();

	auto tr = manager.addComponent<Transform>(player, position, Vector2D(0.0, 0.0), 100.0f, 100.0f, 0.0f, 0.05f);
	(void)tr;
	manager.addComponent<dyn_image>(player, rect_f32{
		{0.0, 0.0},
		{1.0, 1.0}
	}, size2_f32{1.0, 1.0}, manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam, sdlutils().images().at("player"));

	manager.addComponent<MovementController>(player);
	manager.addComponent<ManaComponent>(player);
	std::list<Card*> my_card_list = std::list<Card*>{ new Fireball(), new Lighting(), new Minigun(), new Minigun() };
	manager.addComponent<Deck>(player, my_card_list);
	
	return player;
}
ecs::entity_t create_environment() {
	auto&& manager = *Game::Instance()->get_mngr();
	auto environment = manager.addEntity();
	auto tr = manager.addComponent<Transform>(environment, Vector2D(-16.0, 9.0), Vector2D(0.0, 0.0), 100.0f, 100.0f, 0.0f, 0.05f);
	manager.addComponent<dyn_image>(environment, rect_f32{
		{0.0, 0.0},
		{1.0, 1.0}
		}, size2_f32{ 32.0, 18.0 }, manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam, sdlutils().images().at("floor"));

	return environment;
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
	
	
	
	#pragma endregion
	auto cam = _mngr->addEntity();
	//_mngr->setHandler(ecs::hdlr::CAMERA, cam);
	// auto cam_tr = _mngr->addComponent<Transform>(cam);
	auto &&cam_screen = *_mngr->addComponent<camera_component>(cam, camera_screen{
		.camera = {
			.position = {0.0, 0.0},
			.half_size = {8.0, 4.5},
		},
		.screen = {
			.pixel_size = {sdlutils().width(), sdlutils().height()},
		},
	});
	_mngr->setHandler(ecs::hdlr::CAMERA, cam);
	_game_scene = new GameScene();
	_game_scene->initScene();
	_current_scene = _game_scene;
	create_environment();

	#pragma region player
	auto &&manager = *_mngr;

	auto player = create_test_player_at(Vector2D(0.0, 0.0));
	manager.addComponent<Revolver>(player);

	manager.addComponent<MovementController>(player);
	manager.addComponent<KeyboardPlayerCtrl>(player);
	#pragma endregion



	manager.addComponent<camera_follow>(cam, camera_follow_descriptor{
		.previous_position = cam_screen.cam.camera.position,
		.lookahead_time = 1.0,
		.semi_reach_time = 2.5
	}, cam_screen, *manager.getComponent<Transform>(player));
	manager.addComponent<camera_clamp>(cam, camera_clamp_descriptor{
		.bounds = {
			.position = {0.0, 0.0},
			.size = {32.0, 18.0},
		}
	}, cam_screen);
	return true;
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
	sdlutils().virtualTimer().resetTime();

	while (!exit) {
		// store the current time -- all game objects should use this time when
		// then need to the current time. They also have accessed to the time elapsed
		// between the last two calls to regCurrTime().
		Uint32 startTime = sdlutils().virtualTimer().regCurrTime();
		(void)startTime;

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

		_current_scene->update(sdlutils().virtualTimer().deltaTime());
		_mngr->refresh();


		sdlutils().clearRenderer();
		_current_scene->render();
		sdlutils().presentRenderer();

		//dt = sdlutils().currTime() - startTime;
		//std::cout << sdlutils().currTime() <<" - " <<startTime;
		if (dt < 10) {
			SDL_Delay(10 - dt);
			//dt = 10;
		}
	}

}

ecs::Manager* Game::get_mngr() {
	return _mngr;
}

Scene* Game::get_currentScene() {
	return _current_scene;
}

pair<int, int> Game::get_screen_size() const
{
	return _screen_size;
}

void Game::change_Scene(State nextScene){
	switch (nextScene) {
	case Game::MAINMENU:
		break;	
	case Game::GAMESCENE:
		break;
	case Game::SELECTIONMENU:
		break;
	case Game::NUM_SCENE: {
		assert(false && "unimplemented");
		exit(EXIT_FAILURE);
	}
	default: {
		assert(false && "unreachable");
		exit(EXIT_FAILURE);
	}
	}

}
