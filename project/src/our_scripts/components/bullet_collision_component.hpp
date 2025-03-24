#pragma once
#include "rigidbody_component.hpp"

struct bullet_collision_component : public on_trigger<bullet_collision_component> {
    __CMPID_DECL__(ecs::cmp::BULLET_COLLISION_COMPONENT);

    void on_contact(const trigger_manifold&); //override;
};