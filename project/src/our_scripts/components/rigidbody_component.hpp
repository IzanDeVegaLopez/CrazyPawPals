#ifndef RIGIDBODY_COMPONENT_HPP
#define RIGIDBODY_COMPONENT_HPP

#include "../../ecs/Component.h"
#include "../../ecs/ecs.h"
#include "../../physics/rigidbody.hpp"
#include "Transform.h"
#include "rect_component.hpp"
#include "../../game/Game.h"

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


enum collisionable_option {
    collisionable_option_none = 0,
    collisionable_option_trigger = 1 << 0,
};

typedef uint8_t collisionable_flags;

struct collisionable : public ecs::Component {
    __CMPID_DECL__(ecs::cmp::COLLISIONABLE);

    Transform &transform;
    rigidbody_component &rigidbody;
    rect_component &rect;
    collisionable_flags options;

    collisionable(Transform &transform, rigidbody_component &rigidbody, rect_component &rect, collisionable_option options)
        : transform(transform), rigidbody(rigidbody), rect(rect), options(options) {

    }
};

struct collision_manifold : public ecs::Component {
    __CMPID_DECL__(ecs::cmp::COLLISION_MANIFOLD);

    collision_contact contact;
    ecs::entity_t body0;
    ecs::entity_t body1;

    collision_manifold(collision_contact contact, ecs::entity_t body0, ecs::entity_t body1)
        : contact(contact), body0(body0), body1(body1) {
    }
};

template <typename OnCollisionComponent>
struct on_collision : public ecs::Component {
    using collision_callback = OnCollisionComponent::on_contact;
    void update(uint32_t delta_time) override {
        auto &&manager = *Game::Instance()->get_mngr();
        collision_manifold *manifold = manager.getComponent<collision_manifold>(_ent);
        if (manifold != nullptr) {
            collision_callback(manifold);
            manager.removeComponent<collision_manifold>(_ent);
        }
    }
};

template <typename OnTriggerComponent>
struct on_trigger : public ecs::Component {
    using trigger_callback = OnTriggerComponent::on_contact;
    void update(uint32_t delta_time) override {
        auto &&manager = *Game::Instance()->get_mngr();
        collision_manifold *manifold = manager.getComponent<collision_manifold>(_ent);
        if (manifold != nullptr) {
            trigger_callback(manifold);
            manager.removeComponent<collision_manifold>(_ent);
        }
    }
};


#endif

