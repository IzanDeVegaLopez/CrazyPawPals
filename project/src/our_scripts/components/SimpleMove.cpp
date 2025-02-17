// This file is part of the course TPV2@UCM - Samir Genaim

#include "SimpleMove.h"

#include "../../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "../../ecs/Manager.h"


SimpleMove::SimpleMove() {

}

SimpleMove::~SimpleMove() {
}

void
SimpleMove::initComponent() {
	auto* mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void SimpleMove::update(int delta_time) {
	auto &pos = _tr->getPos();
	auto &dir = _tr->getDir();
	auto speed = _tr->getSpeed();
	pos += dir * speed;
	
	//std::cout << dir;
}
