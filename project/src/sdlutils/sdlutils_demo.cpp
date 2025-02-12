// This file is part of the course TPV2@UCM - Samir Genaim

#include <SDL.h>

#include "sdlutils_demo.h"

#include "InputHandler.h"
#include "macros.h"
#include "SDLUtils.h"
#include "../our_scripts/Container.h"
#include "../our_scripts/KeyboardPlayerCtrl.h"
#include "../our_scripts/Bullet.h"
#include "../our_scripts/Player.h"

#include "../our_scripts/card_system/Deck.hpp"

using namespace std;

void sdlutils_basic_demo() {

	// Initialize the SDLGame singleton
	if (!SDLUtils::Init("SDLGame Demo!", 800, 600,
			"resources/config/sdlutilsdemo.resources.json")) {
		std::cerr << "Something went wrong while initializing SDLUtils!"
				<< std::endl;
		return;
	}

	// Initialize the input handler
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing InputHandler!"
				<< std::endl;
		return;

	}

	// reference to the SDLUtils Singleton. You could use it as a pointer as well,
	// I just prefer to use . instead of ->, it is just a matter of taste, nothing
	// else!
	//
	// you can also use the inline method sdlutils() that simply returns the value
	// of *SDLUtils::instance() --- it is defined at the end of SDLUtils.h
	//
	auto &sdl = *SDLUtils::Instance();

	//show the cursor
	sdl.showCursor();

	// store the 'renderer' in a local variable, just for convenience
	SDL_Renderer *renderer = sdl.renderer();

	/*
	
	*/
	// we can take textures from the predefined ones, and we can create a custom one as well


	//gameObjects
	std::vector<GameObject*> _objs;
	std::vector<GameObject*> _bullets;
	_objs.push_back(new Player(&_bullets));

	
	
	// start the music in a loop
	//sdl.musics().at("beat").play();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto &ih = *InputHandler::Instance();

	// a boolean to exit the loop
	bool exit_ = false;

	//modified---------------------------------------------------------------------------------------------------------------------------------
	Deck deck = Deck(std::list<Card*>{new Card("1"), new Card("2"), new Card("3"), new Card("4")});
	//cout << deck << endl;
	deck.add_card_to_deck(new Fireball());
	deck.add_card_to_deck(new Minigun());
	
	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.reload();
	
	//deck.addCardToDeck(new Card("5"));
	cout << deck << endl;
	//-------------------------------------------------------------------------------------------------------------------------------------------

	while (!exit_) {
		Uint32 startTime = sdl.currRealTime();

		// update the event handler
		ih.refresh();

		// exit when any key is down
		//if (ih.keyDownEvent())
		//	exit_ = true;

		for (auto& o : _objs) {
			o->handleInput();
		}

		// update
		for (auto& o : _objs) {
			o->update();
		}

		for (auto& b : _bullets) {
			b->update();
		}
		// clear screen
		sdl.clearRenderer();

		for (auto& o : _objs) {
			o->render();
		}

		for (auto& b : _bullets) {
			b->render();
		}



		// present new frame
		sdl.presentRenderer();

		Uint32 frameTime = sdl.currRealTime() - startTime;

		deck.update(frameTime);

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

	// stop the music
	Music::haltMusic();

	// Release InputHandler and then SDLUtils (initialization reverse order)
	if (InputHandler::HasInstance())
		InputHandler::Release();
	if (SDLUtils::HasInstance())
		SDLUtils::Release();

}

