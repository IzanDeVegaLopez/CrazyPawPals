#include "Player.h"
#include "../sdlutils/SDLUtils.h"
#include "KeyboardPlayerCtrl.h"
#include "ImageRenderer.h"
#include "Bullet.h"

Player::Player(std::vector<GameObject*>* b): _shootCooldown(0.5f), _lastShoot(0.0f) , b(b){
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
        Vector2D shootDir = (target - _pos).normalize();

		Bullet* bullet = new Bullet(_pos + Vector2D(_width/2,_height/2), shootDir, 20.0f);
		b->push_back(bullet);
        _lastShoot = sdlutils().currRealTime();
    }
}