#include "Player.h"
#include "../sdlutils/SDLUtils.h"
#include "KeyboardPlayerCtrl.h"
#include "ImageRenderer.h"

Player::Player(): _shootCooldown(0.5f), _lastShoot(0.0f) {
	_width = 100.0f;
	_height = 100.0f;
	_pos = {(float)sdlutils().width()/2, (float)sdlutils().height()/2};
	_dir = { 0.0f,0.0f };
	_speed = 5.0f;
	addComponent(new KeyboardPlayerCtrl());
	addComponent(new ImageRenderer(&sdlutils().images().at("gato")));
}

Player::~Player() {}
void

Player::shoot(const Vector2D& target) {
    if (sdlutils().currRealTime() >= _lastShoot + _shootCooldown) {
        _dir = (target - _pos).normalize();
		std::cout << "dispara" << std::endl;
        //Bullet* bullet = new Bullet(playerPos, direction, 5.0f);
        _lastShoot = sdlutils().currRealTime();
    }
}