#include "Camera.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"

Camera::Camera(ecs::Manager* mngr) : GameObject::GameObject(mngr, ecs::grp::CAMERA), _maxSpeed(5.0f)
{
	_tr = _entity->getMngr()->getComponent<Transform>(_entity);
	float s = 100.0f;
	_tr->init({ sdlutils().width() / 2.0f, sdlutils().height() / 2.0f }, { 0.0f,0.0f }, { 0.0f,0.0f }, s, s, 0.0f, 2.0f);
	_entity->getMngr()->addComponent<KeyboardPlayerCtrl>(_entity);
	_entity->getMngr()->addComponent<MovementController>(_entity);
}

Camera::~Camera() {}

Camera::update()
{

}

