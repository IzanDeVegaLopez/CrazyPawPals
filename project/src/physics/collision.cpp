#include "collision.hpp"
#include <cassert>
#include <cstdlib>

// source: adapted from https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
using outcode = uint_fast8_t;

constexpr static const outcode out_code_inside = 0b0000;
constexpr static const outcode out_code_left   = 0b0001;
constexpr static const outcode out_code_right  = 0b0010;
constexpr static const outcode out_code_bottom = 0b0100;
constexpr static const outcode out_code_top    = 0b1000;

struct rect_f32_min_max {
    position2_f32 min;
    position2_f32 max;
};
static outcode rect_f32_min_max_outcode(const rect_f32_min_max rect, const position2_f32 point) {
    outcode code = out_code_inside;
    if (point.x < rect.min.x) {
        code |= out_code_left;
    } else if (point.x > rect.max.x) {
        code |= out_code_right;
    }
    if (point.y < rect.min.y) {
        code |= out_code_bottom;
    } else if (point.y > rect.max.y) {
        code |= out_code_top;
    }
    return code;
}

enum rect_f32_intersection_segment {
    rect_f32_intersection_segment_0 = 0,
    rect_f32_intersection_segment_1,
    rect_f32_intersection_segment_2,
    rect_f32_intersection_segment_enclosed,
};
typedef uint8_t rect_f32_intersection_segment_result;

static rect_f32_intersection_segment_result cohen_sutherland_line_clip(
    const rect_f32_min_max rect,
    const position2_f32 a,
    const position2_f32 b,
    position2_f32 out_clipped[2]
) {
    outcode outcode_a = rect_f32_min_max_outcode(rect, a);
    outcode outcode_b = rect_f32_min_max_outcode(rect, b);

    if (!(outcode_a | outcode_b)) {
        out_clipped[0] = a;
        out_clipped[1] = b;

        return rect_f32_intersection_segment_enclosed;
    } else if (outcode_a & outcode_b) {
        return rect_f32_intersection_segment_0;
    }

    position2_f32 a_clipped = a;
    position2_f32 b_clipped = b;
    uint_fast8_t intersection_count = 0;

    if (outcode_a) {
        const float dx = b.x - a.x;
        const float dy = b.y - a.y;

        if (outcode_a & out_code_top) {
            a_clipped.x += dx * (rect.max.y - a.y) / dy;
            a_clipped.y = rect.max.y;
        } else if (outcode_a & out_code_bottom) {
            a_clipped.x += dx * (rect.min.y - a.y) / dy;
            a_clipped.y = rect.min.y;
        }
        
        if (outcode_a & out_code_right) {
            a_clipped.y += dy * (rect.max.x - a.x) / dx;
            a_clipped.x = rect.max.x;
        } else if (outcode_a & out_code_left) {
            a_clipped.y += dy * (rect.min.x - a.x) / dx;
            a_clipped.x = rect.min.x;
        }
        out_clipped[intersection_count] = a_clipped;
        ++intersection_count;
    }
    
    if (outcode_b) {
        const float dx = a.x - b.x;
        const float dy = a.y - b.y;

        if (outcode_b & out_code_top) {
            b_clipped.x += dx * (rect.max.y - b.y) / dy;
            b_clipped.y = rect.max.y;
        } else if (outcode_b & out_code_bottom) {
            b_clipped.x += dx * (rect.min.y - b.y) / dy;
            b_clipped.y = rect.min.y;
        }
        
        if (outcode_b & out_code_right) {
            b_clipped.y += dy * (rect.max.x - b.x) / dx;
            b_clipped.x = rect.max.x;
        } else if (outcode_b & out_code_left) {
            b_clipped.y += dy * (rect.min.x - b.x) / dx;
            b_clipped.x = rect.min.x;
        }

        out_clipped[intersection_count] = b_clipped;
        ++intersection_count;
    }
    
    switch (intersection_count) {
    case 0: {
        assert(false && "unreachable: if both out_codes were to be 0b0000, we should have taken the both inside case");
        std::exit(EXIT_FAILURE);
    }
    case 1: {
        return rect_f32_intersection_segment_1;
    }
    case 2: {
        return rect_f32_intersection_segment_2;
    }
    default: {
        assert(false && "unreachable: intersection_count should be 0, 1 or 2");
        std::exit(EXIT_FAILURE);
    }
    }
}

size_t rect_f32_intersects_segment(const rect_f32 rect, const position2_f32 a, const position2_f32 b, position2_f32 out_intersections[2]) {
    const size2_f32 half_size = {
        .x = rect.size.x * 0.5f,
        .y = rect.size.y * 0.5f,
    };
    const rect_f32_min_max rect_min_max = {
        .min = {
            .x = rect.position.x - half_size.x,
            .y = rect.position.y - half_size.y,
        },
        .max = {
            .x = rect.position.x + half_size.x,
            .y = rect.position.y + half_size.y,
        },
    };

    const rect_f32_intersection_segment_result result = cohen_sutherland_line_clip(rect_min_max, a, b, out_intersections);
    switch (result) {
    case rect_f32_intersection_segment_0: {
        return 0;
    }
    case rect_f32_intersection_segment_1: {
        return 1;
    }
    case rect_f32_intersection_segment_2: {
        return 2;
    }
    case rect_f32_intersection_segment_enclosed: {
        return 2;
    }
    default: {
        assert(false && "unreachable: result should be 0, 1, 2 or enclosed");
        std::exit(EXIT_FAILURE);
    }
    }
}