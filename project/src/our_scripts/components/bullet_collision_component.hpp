#pragma once
#include "rigidbody_component.hpp"
#include "game/GameStructs.h"

class player_collision_triggerer;
class enemy_collision_triggerer;

struct bullet_collision_component : public on_trigger<bullet_collision_component> {
    GameStructs::collide_with collision_filter;
    int my_damage;
    int pierce_number;
    __CMPID_DECL__(ecs::cmp::BULLET_COLLISION_COMPONENT);
    inline bullet_collision_component(const GameStructs::BulletProperties& bp)
        : collision_filter(bp.collision_filter), my_damage(bp.damage), pierce_number(bp.pierce_number) {}
    //void initComponent() override;
    void on_contact(const collision_manifold&); //override;
    bool check_if_valid_collision(ecs::entity_t);
};