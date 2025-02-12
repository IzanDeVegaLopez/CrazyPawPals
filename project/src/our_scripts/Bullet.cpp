/*
* #include "Bullet.h"
#include "../our_scripts/components/SimpleMove.h"

#include "../sdlutils/SDLUtils.h"
#include <iostream>


Bullet::Bullet(Vector2D& pos, Vector2D& dir, float speed, float lifeTime)
	:_active(true)
{
	_width = 40.0f;
	_height = 40.0f;
	_speed = speed;
	_pos = pos;
	_dir = dir;
	addComponent(new ImageRenderer(&sdlutils().images().at("pacman")));
	addComponent(new SimpleMove());
	_lifeTime = sdlutils().currRealTime() + lifeTime;
}

void
Bullet::update() {
	Container::update();

	if (_pos.getX() < 0 || _pos.getX() > sdlutils().width() ||
		_pos.getY() < 0 || _pos.getY() > sdlutils().height()||
		(sdlutils().currRealTime() > _lifeTime)) {
		_active = false;
	}
}

*/
