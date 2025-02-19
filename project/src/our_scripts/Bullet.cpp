
/*include "Bullet.h"
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

Bullet::~Bullet() {

}

void Bullet::generate_proyectile(BulletProperties& bp, ecs::grpId_t gId)
{
	//TODO create player
	ecs::entity_t new_bullet = Game::Instance()->get_mngr()->addEntity(gId);
	Game::Instance()->get_mngr()->addComponent<Transform>(new_bullet, bp.init_pos, (bp.dir - bp.init_pos).normalize(), bp.width, bp.height, bp.dir.angle(bp.init_pos), bp.speed);
	Game::Instance()->get_mngr()->addComponent<Image>(new_bullet, &sdlutils().images().at("bullet_1"));
	Game::Instance()->get_mngr()->addComponent<SimpleMove>(new_bullet);
	Game::Instance()->get_mngr()->addComponent<LifetimeTimer>(new_bullet, bp.life_time);
}

//void 
*/
