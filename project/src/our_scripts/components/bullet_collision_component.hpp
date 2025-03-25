#pragma once
#include "rigidbody_component.hpp"

class player_collision_triggerer;
class enemy_collision_triggerer;

struct bullet_collision_component : public on_trigger<bullet_collision_component> {
    GameStructs::collide_with collision_filter;
    __CMPID_DECL__(ecs::cmp::BULLET_COLLISION_COMPONENT);
    inline bullet_collision_component(GameStructs::collide_with c = GameStructs::collide_with::none)
        : collision_filter(c) {}
    void on_contact(const trigger_manifold&); //override;
    bool check_if_valid_collision(ecs::entity_t);
};