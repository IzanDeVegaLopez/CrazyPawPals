// This file is part of the course TPV2@UCM - Samir Genaim

#include "BasicEnemy.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"

BasicEnemy::BasicEnemy() :
	_img(&sdlutils().images().at("ninja_standing")), _speed(), _charge() {
	_width = 100;
	_height = 100;
	float x = (sdlutils().width() - _width) / 2;
	float y = sdlutils().height() - _height;
	_pos.set(x, y);
	_state = WALKING;
}

BasicEnemy::~BasicEnemy() {
}

void
BasicEnemy::render() {

}