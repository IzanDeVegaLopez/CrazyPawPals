//#include "bullet.h"
//#include "../our_scripts/components/simplemove.h"
//#include "../our_scripts/components/image.h"
//#include "../our_scripts/components/lifetimetimer.h"
//
//bullet::bullet(ecs::manager* mngr,vector2d& pos, vector2d& dir, vector2d& prevdir, float speed, float lifetime)
//	:gameobject::gameobject(mngr,ecs::grp::bullet)
//{
//	_tr = _entity->getmngr()->getcomponent<transform>(_entity);
//	_tr->init(pos, dir, prevdir, 20.0f, 20.0f, 0.0f, speed);
//	_entity->getmngr()->addcomponent<image>(_entity, &sdlutils().images().at("pacman"));
//	_entity->getmngr()->addcomponent<simplemove>(_entity);
//	_entity->getmngr()->addcomponent<lifetimetimer>(_entity,lifetime);
//}
//
//bullet::~bullet() {
//
//}
//
//void bullet::generate_proyectile(bulletproperties& bp, ecs::grpid_t gid)
//{
//	//todo create player
//	ecs::entity_t new_bullet = game::instance()->get_mngr()->addentity(gid);
//	game::instance()->get_mngr()->addcomponent<transform>(new_bullet, bp.init_pos, (bp.dir - bp.init_pos).normalize(), bp.width, bp.height, bp.dir.angle(bp.init_pos), bp.speed);
//	game::instance()->get_mngr()->addcomponent<image>(new_bullet, &sdlutils().images().at("bullet_1"));
//	game::instance()->get_mngr()->addcomponent<simplemove>(new_bullet);
//	game::instance()->get_mngr()->addcomponent<lifetimetimer>(new_bullet, bp.life_time);
//}
//
////void 
//
