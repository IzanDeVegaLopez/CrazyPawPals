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

    if (check_if_valid_collision(entity_collided_with)) {
        auto&& manager = *Game::Instance()->get_mngr();
        if (manager.hasComponent<Health>(entity_collided_with)) {
            std::cout << my_damage << std::endl;
            auto health = manager.getComponent<Health>(entity_collided_with);
            health->takeDamage(my_damage);
            Game::Instance()->get_mngr()->setAlive(_ent, pierce_number-- > 0);
        }
        std::cout << "UwU" << std::endl;
    }
    else {
        std::cout << "Nop" << std::endl;
    }
}

bool paw_patrol_collision_component::check_if_valid_collision(ecs::entity_t ent_col)
{
    switch (collision_filter)
    {
    case GameStructs::collide_with::player:
        return Game::Instance()->get_mngr()->hasComponent<player_collision_triggerer>(ent_col);
        break;
    case GameStructs::collide_with::none:
        return false;
        break;
    default:
        return false;
    }
}
