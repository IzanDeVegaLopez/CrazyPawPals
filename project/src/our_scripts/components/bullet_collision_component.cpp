#include "bullet_collision_component.hpp"
#include "rigidbody_component.hpp"
#include "collision_triggerers.hpp"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../../game/GameStructs.h"

void bullet_collision_component::on_contact(const trigger_manifold& tm)
{
    //takes pointer to the other body
    ecs::entity_t entity_collided_with = _ent==tm.body0 ? tm.body1 : tm.body0;

    if (check_if_valid_collision(entity_collided_with)) {
        Game::Instance()->get_mngr()->setAlive(_ent, false);
        std::cout << "bala shine" << std::endl;
    }
}

bool bullet_collision_component::check_if_valid_collision(ecs::entity_t ent_col)
{
    switch (collision_filter)
    {
    case GameStructs::collide_with::player:
        return Game::Instance()->get_mngr()->hasComponent<player_collision_triggerer>(ent_col);
        break;
    case GameStructs::collide_with::enemy:
        return Game::Instance()->get_mngr()->hasComponent<enemy_collision_triggerer>(ent_col);
        break;
    case GameStructs::collide_with::all:
        return true;
        break;
    case GameStructs::collide_with::none:
        return false;
        break;
    default:
        return false;
    }
}
