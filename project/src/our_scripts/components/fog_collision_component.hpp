#pragma once
#include "rigidbody_component.hpp"
#include "game/GameStructs.h"

class player_collision_triggerer;
class enemy_collision_triggerer;

struct fog_collision_component : public on_trigger<fog_collision_component> {
    __CMPID_DECL__(ecs::cmp::FOG_COLLISION_COMPONENT);

    int fogDamage = 1;
    bool damagingPlayer = false;
    Uint32 fogCollidingTimer;
    
    inline fog_collision_component() : on_trigger<fog_collision_component>() {};
    void update(uint32_t delta_time) override;
    void on_contact(const trigger_manifold&);
};