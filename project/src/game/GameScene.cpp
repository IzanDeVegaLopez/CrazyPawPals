#include "GameScene.h"

void GameScene::update()
{
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

		_mngr->update();
		_mngr->refresh();

		checkCollisions();

		sdlutils().clearRenderer();
		_mngr->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
}
