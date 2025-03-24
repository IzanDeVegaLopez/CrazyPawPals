#include "bullet_collision_component.hpp"
#include "rigidbody_component.hpp"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
void bullet_collision_component::on_contact(const trigger_manifold& )
{
    Game::Instance()->get_mngr()->setAlive(_ent, false);
    std::cout << "bala shine" << std::endl;
}
