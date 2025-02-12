#include "Bullet.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/LifetimeTimer.h"

Bullet::Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, float speed, float lifeTime)
	:GameObject::GameObject(mngr,ecs::grp::BULLET)
{
	_tr = _mngr->getComponent<Transform>(_entity);
	_tr->init(pos, dir, 40.0f, 40.0f, 0.0f, speed);
	_mngr->addComponent<Image>(_entity, &sdlutils().images().at("pacman"));
	_mngr->addComponent<SimpleMove>(_entity);
	_mngr->addComponent<LifetimeTimer>(_entity,lifeTime);
}
