#ifndef RIGIDBODY_COMPONENT_HPP
#define RIGIDBODY_COMPONENT_HPP

#include "../../ecs/Component.h"
#include "../../physics/rigidbody.hpp"
#include "Transform.h"
#include "rect_component.hpp"

#ifndef RIGIDBODY_COMPONENT_DEGEN_RESTITUTION_OFF
#define RIGIDBODY_COMPONENT_DEGEN_RESTITUTION true
#else
#define RIGIDBODY_COMPONENT_DEGEN_RESTITUTION false
#endif

struct rigidbody_component : public ecs::Component {
    __CMPID_DECL__(ecs::cmp::RIGIDBODY);
    inverse_mass_f32 inverse_mass;
    float restitution_coefficient;

private:
    rigidbody_component()
        : inverse_mass{0.0f}, restitution_coefficient{0.0} {

    }

public:
    rigidbody_component(mass_f32 mass, float restitution_coefficient);
    rigidbody_component(inverse_mass_f32 inverse_mass, float restitution_coefficient);
};


struct collisionable : public ecs::Component {
    __CMPID_DECL__(ecs::cmp::COLLISIONABLE);

    Transform &transform;
    rigidbody_component &rigidbody;
    rect_component &rect;
    bool trigger;

    collisionable(Transform &transform, rigidbody_component &rigidbody, rect_component &rect, bool trigger)
        : transform(transform), rigidbody(rigidbody), rect(rect), trigger(trigger) {

    }
};


#endif