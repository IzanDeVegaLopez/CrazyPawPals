// This file is part of the course TPV2@UCM - Samir Genaim

#include "../game/NinjaWS.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "AttackingState.h"
#include "DuckingState.h"
#include "JumpingState.h"
#include "StandingState.h"
#include "WalkingState.h"

NinjaWS::NinjaWS() :
		_img(), //
		_speed(), //
		_charge(), //
		_state(), //
		_standig_state(), //
		_jumping_state(), //
		_walking_state(), //
		_ducking_state(), //
		_attacking_state() //
{
	_width = 100;
	_height = 100;
	float x = (sdlutils().width() - _width) / 2;
	float y = sdlutils().height() - _height;
	_pos.set(x, y);

	_standig_state = new StandingState();
	_jumping_state = new JumpingState();
	_walking_state = new WalkingState();
	_ducking_state = new DuckingState();
	_attacking_state = new AttackingState();

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

