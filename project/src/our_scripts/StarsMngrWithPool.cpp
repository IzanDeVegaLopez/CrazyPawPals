// This file is part of the course TPV2@UCM - Samir Genaim

#include "StarsMngrWithPool.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
StarsMngrWithPool::StarsMngrWithPool() :
		_pool(50), //
		_img(&sdlutils().images().at("star")) //
{
}

StarsMngrWithPool::~StarsMngrWithPool() {
}
//
//void StarsMngrWithPool::update() {
//	for (auto o : _pool) {
//		if (_pool.is_used(o)) {
//			o->_width *= 0.9f;
//			o->_height *= 0.9f;
//			if (o->_width < 1 && o->_height < 1) {
//				_pool.free(o);
//			}
//		}
//	}
//
//}
//
//void StarsMngrWithPool::render() {
//	for (auto o : _pool) {
//		if (_pool.is_used(o)) {
//			SDL_Rect dest = build_sdlrect(o->_pos, o->_width, o->_height);
//			_img->render(dest);
//		}
//	}
//}
//
//void StarsMngrWithPool::handleInput() {
//
//	auto &ihdlr = ih();
//
//	if (ihdlr.keyDownEvent()) {
//		if (ihdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
//			addStarts(1);
//		}
//	}

//}

void StarsMngrWithPool::addStarts(uint16_t n) {
	
	/*
	for (auto i = 0u; i < n; i++) {
		Star *s = _pool.alloc();
		if (s == nullptr)
			break;
		auto &rand = sdlutils().rand();
		s->_width = s->_height = rand.nextInt(30, 100);
		s->_pos.set(rand.nextInt(0, sdlutils().height()),
				rand.nextInt(0, sdlutils().width()));
	}
	
	*/
	
}
