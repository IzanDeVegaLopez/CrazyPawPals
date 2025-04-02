#include "fog_collision_component.hpp"
#include "rigidbody_component.hpp"
#include "collision_triggerers.hpp"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../../game/GameStructs.h"
#include "Health.h"
#include "collision_registration_by_id.h"
#include "id_component.h"
#include "../../sdlutils/SDLUtils.h"

void fog_collision_component::on_contact(const collision_manifold& tm)
{
    //takes pointer to the other body
    ecs::entity_t entity_collided_with = _ent == tm.body0 ? tm.body1 : tm.body0;
    if (Game::Instance()->get_mngr()->hasComponent<player_collision_triggerer>(entity_collided_with)
        /*|| entity_collided_with == Game::Instance()->get_mngr()->getHandler(ecs::hdlr::PLAYER)*/) {
        
        //std::cout << "entity_collided_with: " << entity_collided_with << ", Game::Instance()->get_mngr()->getHandler(ecs::hdlr::PLAYER): " << Game::Instance()->get_mngr()->getHandler(ecs::hdlr::PLAYER) << std::endl;
        fogCollidingTimer = sdlutils().virtualTimer().currRealTime();
        std::cout << Game::Instance()->get_mngr()->hasComponent<player_collision_triggerer>(entity_collided_with) << std::endl;
        std::cout << "1 vT: " << sdlutils().virtualTimer().currRealTime() << " fogCT: " << fogCollidingTimer << std::endl;
    }
}

void fog_collision_component::update(uint32_t delta_time) {
	on_trigger<fog_collision_component>::update(delta_time);
	if (sdlutils().virtualTimer().currRealTime() > fogCollidingTimer + 500) {
		damagingPlayer = true;
        std::cout << (damagingPlayer ? "" : "NOT ") << "damaging player" << std::endl;
	}
    else
    {
		damagingPlayer = false;
    }
}

