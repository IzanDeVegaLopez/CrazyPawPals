#pragma once
#include "rigidbody_component.hpp"
#include "game/GameStructs.h"

class player_collision_triggerer;
class enemy_collision_triggerer;

struct fog_collision_component : public on_trigger<fog_collision_component> {
    GameStructs::collide_with collision_filter;
    int my_damage;
    int pierce_number;
    __CMPID_DECL__(ecs::cmp::FOG_COLLISION_COMPONENT);
    inline fog_collision_component() {};
    //void initComponent() override;
    void on_contact(const trigger_manifold&); //override;
    bool check_if_valid_collision(ecs::entity_t);
};