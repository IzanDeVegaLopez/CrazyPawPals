// This file is part of the course TPV2@UCM - Samir Genaim

#include <SDL.h>

#include "sdlutils_demo.h"

#include "InputHandler.h"
#include "macros.h"
#include "SDLUtils.h"

#include "rendering/card_rendering.hpp"

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

	// we can take textures from the predefined ones, and we can create a custom one as well
	auto &sdlLogo = sdl.images().at("sdl_logo");
	auto &helloSDL = sdl.msgs().at("HelloSDL");
	Texture pressAnyKey(renderer, "Press any key to exit",
			sdl.fonts().at("ARIAL24"), build_sdlcolor(0x112233ff),
			build_sdlcolor(0xffffffff));

	// some coordinates
	auto winWidth = sdl.width();
	auto winHeight = sdl.height();
	auto x0 = (winWidth - pressAnyKey.width()) / 2;
	auto y0 = (winHeight - pressAnyKey.height()) / 2;
	auto x1 = 0;
	auto y1 = y0 - 4 * pressAnyKey.height();
	auto x2 = (winWidth - sdlLogo.width()) / 2;
	auto y2 = y0 + 2 * pressAnyKey.height();

	// start the music in a loop
	sdl.musics().at("beat").play();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto &ih = *InputHandler::Instance();

	const card_rendering_descriptor descriptors[4] = {
		card_rendering_descriptor{{{0.0f, 0.0f,}, {0.2f, 0.2f}}, "test_card_sheet", "ARIAL16", SDL_Color{255, 255, 255, 255}, 1},
		card_rendering_descriptor{{{0.0f, 0.0f,}, {0.2f, 0.2f}}, "test_card_sheet", "ARIAL16", SDL_Color{255, 255, 255, 255}, 2},
		card_rendering_descriptor{{{0.0f, 0.0f,}, {0.2f, 0.2f}}, "test_card_sheet", "ARIAL16", SDL_Color{255, 255, 255, 255}, 3},
		card_rendering_descriptor{{{0.0f, 0.0f,}, {0.2f, 0.2f}}, "test_card_sheet", "ARIAL16", SDL_Color{255, 255, 255, 255}, 4}
	};
	camera_screen camera = {
		.camera = {
			.position = {0, 0},
			.half_size = {16, 9}
		},
		.screen = {
			.pixel_size = {winWidth, winHeight}
		}
	};
	// a boolean to exit the loop
	bool exit_ = false;
	while (!exit_) {
		Uint32 startTime = sdl.currRealTime();

		// update the event handler
		ih.refresh();

		// exit when any key is down
		if (ih.keyDownEvent())
			exit_ = true;

		// clear screen
		sdl.clearRenderer();

		// render Hello SDL
		helloSDL.render(x1, y1);
		if (x1 + helloSDL.width() > winWidth)
			helloSDL.render(x1 - winWidth, y1);
		x1 = (x1 + 5) % winWidth;

		
		
		// render Press Any Key
		pressAnyKey.render(x0, y0);
		
		// render the SDLogo
		sdlLogo.render(x2, y2);
		for (size_t i = 0; i < 4; ++i) {
			const SDL_Rect destination = card_rendering_descriptor_render(
				descriptors[i],
				camera,
				*renderer,
				rect_f32{
					.position = { float(i) * 2.0f, 0.0f },
					.size = { 2.0f, 2.0f },
				},
				rect_f32{
					.position = { float(i) / 4.0f, 0.0f },
					.size = { 1.0f / 4.0f, 1.0f },
				},
				0.0f,
				card_rendering_descriptor_options_none
			);

			std::cout << "destination: " << destination.x << ", " << destination.y << ", " << destination.w << ", " << destination.h << std::endl;
		}
		
		// present new frame
		sdl.presentRenderer();

		Uint32 frameTime = sdl.currRealTime() - startTime;

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

