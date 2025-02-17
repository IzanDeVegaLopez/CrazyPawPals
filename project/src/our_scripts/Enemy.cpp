#include "Enemy.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/EnemyMovement.h"
#include "../our_scripts/components/MovementController.h"
#include "../ecs/Entity.h"

Enemy::Enemy(ecs::Manager* mngr)
	:GameObject::GameObject(mngr,ecs::grp::ENEMY)
{
	_tr = _entity->getMngr()->getComponent<Transform>(_entity);
	_entity->getMngr()->addComponent<Image>(_entity, &sdlutils().images().at("enemy"));
	_entity->getMngr()->addComponent<MovementController>(_entity);
	_entity->getMngr()->addComponent<SimpleMove>(_entity);
	_entity->getMngr()->addComponent<EnemyMovement>(_entity);

	_img = _entity->getMngr()->getComponent<Image>(_entity);
	
	float s = 100.0f;
	float x = (sdlutils().width() - s) / 2.0f;
	float y = (sdlutils().height() - s) / 2.0f;
	_tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f, 0.5f);
}

void Enemy::init(const Vector2D& pos, float width, float height, float speed, const std::string& imageName) {
	_img->setTexture(&sdlutils().images().at(imageName));
	_tr->init(pos, Vector2D(), width, height, 0.0f, speed);
}

Image*
Enemy::getImage() {
	return _img;
}

ecs::entity_t
Enemy::getEntity() {
	return _entity;
}

Transform*
Enemy::getTransform() {
	return _tr;
}