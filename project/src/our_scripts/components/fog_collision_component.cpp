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

void fog_collision_component::on_contact(const trigger_manifold& tm)
{
    //takes pointer to the other body
    ecs::entity_t entity_collided_with = _ent==tm.body0 ? tm.body1 : tm.body0;
    if (entity_collided_with == Game::Instance()->get_mngr()->getHandler(ecs::hdlr::PLAYER)) {
        fogCollidingTimer = sdlutils().virtualTimer().currRealTime();
    }
}

void fog_collision_component::update(uint32_t delta_time) {
	on_trigger<fog_collision_component>::update(delta_time);
	if (sdlutils().virtualTimer().currRealTime() > fogCollidingTimer + 500) {
		damagingPlayer = true;
	}
    else
    {
		damagingPlayer = false;
    }

	//std::cout << "vT: " << sdlutils().virtualTimer().currRealTime() << " fogCT: " << fogCollidingTimer << std::endl;
    std::cout << (damagingPlayer ? "" : "NOT ") << "damaging with player" << std::endl;
}