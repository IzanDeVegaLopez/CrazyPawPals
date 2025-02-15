#include "Bullet.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/LifetimeTimer.h"

Bullet::Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, Vector2D& prevDir, float speed, float lifeTime)
	:GameObject::GameObject(mngr,ecs::grp::BULLET)
{
	_tr = _entity->getMngr()->getComponent<Transform>(_entity);
	_tr->init(pos, dir, prevDir, 20.0f, 20.0f, 0.0f, speed);
	_entity->getMngr()->addComponent<Image>(_entity, &sdlutils().images().at("pacman"));
	_entity->getMngr()->addComponent<SimpleMove>(_entity);
	_entity->getMngr()->addComponent<LifetimeTimer>(_entity,lifeTime);
}
