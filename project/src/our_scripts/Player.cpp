//#include "Player.h"
//#include "../sdlutils/SDLUtils.h"
//#include "../ecs/Manager.h"
//
//#include "../our_scripts/components/Image.h"
//#include "../our_scripts/components/KeyboardPlayerCtrl.h"
//#include "../our_scripts/components/MovementController.h"
//#include "../our_scripts/components/ShootComponent.h"
//#include "../our_scripts/components/Health.h"
//#include "../our_scripts/components/Revolver.h"
//
//
//using namespace ecs;
//
//Player::Player(ecs::Manager* mngr) : GameObject::GameObject(mngr, ecs::grp::PLAYER),_shootCooldown(0.5f){
//
//	_tr = _entity->getMngr()->getComponent<Transform>(_entity);
//	float s = 100.0f;
//	_tr->init({ sdlutils().width() / 2.0f, sdlutils().height() / 2.0f }, { 0.0f,0.0f }, { 0.0f,0.0f }, s, s, 0.0f, 2.0f);
//	_entity->getMngr()->addComponent<Image>(_entity, &sdlutils().images().at("player"));
//	_entity->getMngr()->addComponent<Revolver>(_entity);
//	_entity->getMngr()->addComponent<Health>(_entity, 20);
//	_entity->getMngr()->addComponent<KeyboardPlayerCtrl>(_entity);
//	_entity->getMngr()->addComponent<MovementController>(_entity);
//}
//
///*
//* Player::Player(std::vector<ecs::Entity*>* b) : ecs::entity (),_shootCooldown(0.5f), _lastShoot(0.0f), _maxSpeed(10.0f), _prevDir({ 0.0f,0.0f }), b(b) {
//	_width = 100.0f;
//	_height = 100.0f;
//	_pos = {(float)sdlutils().width()/2 - _width/2, (float)sdlutils().height()/2 - _height/2};
//	_dir = { 0.0f,0.0f };
//	_speed = 5.0f;
//
//	addComponent(new KeyboardPlayerCtrl());
//	addComponent(new ImageRenderer(&sdlutils().images().at("gato")));
//	addComponent(new MovementController());
//}
//
//
//Player::~Player() {}
//
//void
//Player::shoot(const Vector2D& target) {
//	if (sdlutils().currRealTime() >= _lastShoot + _shootCooldown) {
//		Vector2D shootDir = (target - _pos).normalize();
//
//		Bullet* bullet = new Bullet(_pos + Vector2D(_width/2,_height/2), shootDir, 20.0f);
//		b->push_back(bullet);
//		_lastShoot = sdlutils().currRealTime();
//	}
//}
//
//void
//Player::move(){
//	//We change the position accoding to the inputs we've recieved from keyboard control
//	if (_dir.getX() != 0 && _dir.getY() != 0) _dir = _dir.normalize(); //If its a diagonal movement, normalize dir
//	_pos = _pos + (_dir * _speed);
//
//}
//*/
//
