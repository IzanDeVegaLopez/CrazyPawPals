#pragma once
#include "rigidbody_component.hpp"
#include "game/GameStructs.h"

class player_collision_triggerer;
class enemy_collision_triggerer;

struct paw_patrol_collision_component : public on_collision<paw_patrol_collision_component> {
    int my_damage;
    __CMPID_DECL__(ecs::cmp::PAW_PATROL_COLLISION_COMPONENT);
    inline paw_patrol_collision_component(const int dmg, GameStructs::collide_with col)
        : my_damage(dmg) {}
    //void initComponent() override;
    void on_contact(const collision_manifold&); //override;
};