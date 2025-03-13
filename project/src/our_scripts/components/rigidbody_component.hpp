#ifndef RIGIDBODY_COMPONENT_HPP
#define RIGIDBODY_COMPONENT_HPP

#include "../../ecs/Component.h"
#include "../../ecs/ecs.h"
#include "../../physics/rigidbody.hpp"
#include "../../physics/collision.hpp"
#include "Transform.h"
#include "rect_component.hpp"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"

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

template <ecs::cmp::cmpId COMPONENT_ID>
struct collision_manifold : public ecs::Component {
    __CMPID_DECL__(COMPONENT_ID);

    collision_contact contact;
    ecs::entity_t body0;
    ecs::entity_t body1;

    collision_manifold(collision_contact contact, ecs::entity_t body0, ecs::entity_t body1)
        : contact(contact), body0(body0), body1(body1) {
    }
};
using contact_manifold = collision_manifold<ecs::cmp::CONTACT_MANIFOLD>;
using trigger_manifold = collision_manifold<ecs::cmp::TRIGGER_MANIFOLD>;

template <typename OnCollisionComponent>
struct on_collision : public ecs::Component {
    using collision_callback = OnCollisionComponent::on_contact;
    void update(uint32_t delta_time) override {
        auto &&manager = *Game::Instance()->get_mngr();
        contact_manifold *manifold = manager.getComponent<contact_manifold>(_ent);
        if (manifold != nullptr) {
            collision_callback(*manifold);
            manager.removeComponent<collision_manifold>(_ent);
        }
    }
};

template <typename OnTriggerComponent>
struct on_trigger : public ecs::Component {
    using trigger_callback = OnTriggerComponent::on_contact;
    void update(uint32_t delta_time) override {
        auto &&manager = *Game::Instance()->get_mngr();
        trigger_manifold *manifold = manager.getComponent<trigger_manifold>(_ent);
        if (manifold != nullptr) {
            trigger_callback(*manifold);
            manager.removeComponent<collision_manifold>(_ent);
        }
    }
};

// DOC!

// class test_collision_callback : public on_collision<test_collision_callback> {
//     __CMPID_DECL__(ecs::cmp::/*COMP_ID*/);
//     void on_contact(const contact_manifold &manifold) {
//         // Collision event code here
//     }
// };

// class test_trigger_callback : public on_trigger<test_trigger_callback> {
//     __CMPID_DECL__(ecs::cmp::/*COMP_ID*/);
//     void on_contact(const trigger_manifold &manifold) {
//         // Triiger event code here
//     }
// };

#endif

