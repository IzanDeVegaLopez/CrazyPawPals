// This file is part of the course TPV2@UCM - Samir Genaim

#include "SimpleMove.h"

#include "../../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "../../ecs/Manager.h"
#include "../../game/Game.h"

SimpleMove::SimpleMove() {

}

SimpleMove::~SimpleMove() {
}

void
SimpleMove::initComponent() {

	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void SimpleMove::update(uint32_t delta_time) {
	auto &pos = _tr->getPos();
	auto &dir = _tr->getDir();
	auto speed = _tr->getSpeed();
	pos += dir * speed;
	
	//std::cout << dir;
}
