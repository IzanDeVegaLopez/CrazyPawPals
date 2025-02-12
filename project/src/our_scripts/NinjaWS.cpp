// This file is part of the course TPV2@UCM - Samir Genaim

#include "../our_scripts/NinjaWS.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"


NinjaWS::NinjaWS() 
	
{
	_width = 100;
	_height = 100;
	float x = (sdlutils().width() - _width) / 2;
	float y = sdlutils().height() - _height;
	_pos.set(x, y);

	/*_standig_state = new StandingState();
	_jumping_state = new JumpingState();
	_walking_state = new WalkingState();
	_ducking_state = new DuckingState();
	_attacking_state = new AttackingState();*/

	_state = _standig_state;
	_state->enter(*this);
}

NinjaWS::~NinjaWS() {
}

void NinjaWS::update() {
	_state->update(*this);
}

void NinjaWS::handleInput() {
	_state->handleInput(*this);
}

void NinjaWS::render() {
	SDL_Rect rect = build_sdlrect(_pos, _width, _height);

	_img->render(rect);

}

