#include "Enemy.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/EnemyMovement.h"
#include "../our_scripts/components/MovementController.h"

Enemy::Enemy(ecs::Manager* mngr)
	:GameObject::GameObject(mngr,ecs::grp::ENEMY)
{
	_tr = _entity->getMngr()->getComponent<Transform>(_entity);
	_entity->getMngr()->addComponent<Image>(_entity, &sdlutils().images().at("player"));
	_entity->getMngr()->addComponent<MovementController>(_entity);
	_entity->getMngr()->addComponent<SimpleMove>(_entity);
	_entity->getMngr()->addComponent<EnemyMovement>(_entity);

	
	float s = 100.0f;
	float x = (sdlutils().width() - s) / 2.0f;
	float y = (sdlutils().height() - s) / 2.0f;
	_tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f, 0.5f);
}
