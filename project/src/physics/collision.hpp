#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "../rendering/rect.hpp"

size_t rect_f32_intersects_segment(const rect_f32 rect, const position2_f32 a, const position2_f32 b, position2_f32 out_intersections[2]);

#endif