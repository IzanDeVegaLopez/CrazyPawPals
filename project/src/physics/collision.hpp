#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "../rendering/rect.hpp"
#include "../rendering/units.hpp"
#include "rigidbody.hpp"

size_t rect_f32_intersects_segment(
    const rect_f32 rect,
    const position2_f32 a,
    const position2_f32 b,
    position2_f32 out_intersections[2]
);

struct collision_body {
    rigidbody body;
    float restitution;
};

struct collision_penetration {
    vec2_f32 penetration;
};

struct collision_contact {
    collision_penetration penetration;
    vec2_f32 normal;
};

bool collision_body_check(
    const collision_body &moving,
    const collision_body &stationary,
    const seconds_f32 delta_time,
    collision_contact &out_contact
);

void collision_body_resolve(
    collision_body &moving,
    collision_body &stationary,
    const seconds_f32 delta_time,
    const collision_contact &contact
);

#endif