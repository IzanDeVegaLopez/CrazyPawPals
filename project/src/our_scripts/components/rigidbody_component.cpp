#include "rigidbody_component.hpp"
#include <cassert>

void verlet_component::update(uint32_t delta_time_milliseconds) {
    auto &&position = transform.getPos();
    previous_position = position2_f32{
        .x = position.getX(),
        .y = position.getY()
    };
    transform.update(delta_time_milliseconds);
}

rigidbody_component::rigidbody_component(mass_f32 mass, float restitution_coefficient){
    assert(mass.value > 0.0f && "error: must be strictly positive");
    assert(restitution_coefficient >= 0.0f && "error: restitution must be non-negative");
#if !RIGIDBODY_COMPONENT_DEGEN_RESTITUTION
    assert(restitution_coefficient <= 1.0f && "error: restitution must be less or equal to one if the RIGIDBODY_COMPONENT_DEGEN_RESTITUTION flag is enabled");
#endif
    inverse_mass = {1.0f / mass.value};
    restitution_coefficient = restitution_coefficient;
}

rigidbody_component::rigidbody_component(inverse_mass_f32 inverse_mass, float restitution_coefficient) 
    : inverse_mass{inverse_mass} {

#if !RIGIDBODY_COMPONENT_DEGEN_RESTITUTION
    assert(restitution_coefficient <= 1.0f && "error: restitution must be less or equal to one if the RIGIDBODY_COMPONENT_DEGEN_RESTITUTION flag is enabled");
#endif
    restitution_coefficient = restitution_coefficient;
}
