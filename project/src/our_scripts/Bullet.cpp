#include "Bullet.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/LifetimeTimer.h"

Bullet::Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, float speed, float lifeTime)
	:GameObject::GameObject(mngr,ecs::grp::BULLET)
{
	_tr = _entity->getMngr()->getComponent<Transform>(_entity);
	_entity->getMngr()->addComponent<Image>(_entity, &sdlutils().images().at("bullet_1"));
	_entity->getMngr()->addComponent<SimpleMove>(_entity);
	_entity->getMngr()->addComponent<LifetimeTimer>(_entity,lifeTime);

	_tr->setHeight(40.0f);
	_tr->setWidth(40.0f);
	_tr->setSpeed(speed);
	_tr->setDir(dir);
	_tr->setPos(pos);
}
