#include "Bullet.h"
#include "SimpleMove.h"
#include "ImageRenderer.h"

#include "../sdlutils/SDLUtils.h"

#include <iostream>

Bullet::Bullet(Vector2D& pos, Vector2D& dir, float speed)
	:_active(true),_speed(speed)
{
	_width = 40.0f;
	_height = 40.0f;
	_pos = pos;
	_dir = dir;
	addComponent(new ImageRenderer(&sdlutils().images().at("pacman")));
	addComponent(new SimpleMove());

}

void
Bullet::update() {
	Container::update();

	//std::cout << _pos << std::endl;
	if (_pos.getX() < 0 || _pos.getX() > sdlutils().width() ||
		_pos.getY() < 0 || _pos.getY() > sdlutils().height()) {
		_active = false;
	}
}
