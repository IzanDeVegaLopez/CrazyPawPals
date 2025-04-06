#include "paw_patrol_collision_component.hpp"
#include "rigidbody_component.hpp"
#include "collision_triggerers.hpp"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../../game/GameStructs.h"
#include "Health.h"
#include "collision_registration_by_id.h"
#include "id_component.h"
#include "rigidbody_component.hpp"

void paw_patrol_collision_component::on_contact(const collision_manifold& tm)
{
    //takes pointer to the other body
    ecs::entity_t entity_collided_with = _ent == tm.body0 ? tm.body1 : tm.body0;

    if (Game::Instance()->get_mngr()->hasComponent<player_collision_triggerer>(entity_collided_with)) {
        auto&& manager = *Game::Instance()->get_mngr();
        if (manager.hasComponent<Health>(entity_collided_with)) {
            auto health = manager.getComponent<Health>(entity_collided_with);
            health->takeDamage(my_damage);
            //Game::Instance()->get_mngr()->setAlive(_ent, health->getHealth() > 0);
        }
    }
}