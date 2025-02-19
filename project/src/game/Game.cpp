// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"


#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/Transform.h"
#include "../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../our_scripts/components/MovementController.h"
#include "../our_scripts//components/ShootComponent.h"
#include "../our_scripts//components/SimpleMove.h"
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/Bullet.h"
#include "../our_scripts/components/dyn_image.hpp"
#include "../our_scripts/components/camera_component.hpp"


using namespace std;


Game::Game() :
	_mngr(nullptr) {
}

Game::~Game() {
	delete _mngr;

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

	auto tr = manager.addComponent<Transform>(player);
	float s = 100.0f;
	tr->init(position, Vector2D(0.0, 0.0), s, s, 0.0f, 1.0);
	manager.addComponent<dyn_image>(player, rect_f32{
		{0.0, 0.0},
		{1.0, 1.0}
	}, size2_f32{1.0, 1.0}, manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam, sdlutils().images().at("player"));

	manager.addComponent<ShootComponent>(player);
	//manager.addComponent<MovementController>(player);
	manager.addComponent<Mana>(player);
	std::list<Card*> my_card_list = std::list<Card*>{ new Fireball(), new Fireball(), new Minigun(), new Minigun() };
	manager.addComponent<Deck>(player, my_card_list);
	manager.addComponent<KeyboardPlayerCtrl>(player);

	return player;
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

	// Habilitar el cursor del rat�n
	SDL_ShowCursor(SDL_ENABLE);

	// Create the manager
	_mngr = new ecs::Manager();

#pragma region bullets
	std::vector<Bullet*> b;
	/*
		for (int i = 0; i < 100; ++i) {
		auto ins = _mngr->addEntity();
		auto tr = _mngr->addComponent<Transform>(ins);
		float s = 20.0f;
		float x = -1.0f;
		float y = -1.0f;
		tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f, 2.0f);
		_mngr->addComponent<SimpleMove>(ins);
		b.push_back(ins);
	}
	*/

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

#pragma region player
	auto &&manager = *_mngr;
	auto player = create_test_player_at(Vector2D(0.0, 0.0));
	manager.addComponent<camera_follow>(cam, camera_follow_descriptor{
		.previous_position = cam_screen.cam.camera.position,
		.lookahead_time = 1.0,
		.semi_reach_time = 2.5
	}, *manager.getComponent<Transform>(player));
	manager.addComponent<MovementController>(player);

	create_test_player_at(Vector2D(4.0f, 0.0f));
	create_test_player_at(Vector2D(-4.0f, 0.0f));
	create_test_player_at(Vector2D(0.0f, 4.0f));
	create_test_player_at(Vector2D(0.0f, -4.0f));

#pragma endregion
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

		_mngr->update(dt);
		_mngr->refresh();

		checkCollisions();

		sdlutils().clearRenderer();
		_mngr->render();
		sdlutils().presentRenderer();

		dt = sdlutils().currRealTime() - startTime;
		if (dt < 10)
			SDL_Delay(10 - dt);
	}

}

ecs::Manager* Game::get_mngr() {
	return _mngr;
}

void Game::checkCollisions() {


}

