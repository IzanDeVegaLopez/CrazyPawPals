#include "Bullet.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/LifetimeTimer.h"
#include <cmath> //for PI

Bullet::Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, Vector2D& prevDir, float speed, float lifeTime, const std::string& texName, int width, int height)
	:GameObject::GameObject(mngr,ecs::grp::BULLET)
{
	_tr = _entity->getMngr()->getComponent<Transform>(_entity);
	auto tex = &sdlutils().images().at(texName);
	float angle = atan2(dir.getY(), dir.getX()) * 180.0f / M_PI;
	_tr->init(pos, dir, prevDir, width, height, angle, speed);
	_entity->getMngr()->addComponent<Image>(_entity, tex);
	_entity->getMngr()->addComponent<SimpleMove>(_entity);
	_entity->getMngr()->addComponent<LifetimeTimer>(_entity,lifeTime);
}
