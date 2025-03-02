#include "collision.hpp"

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
static outcode rect_f32_min_max_outcode(rect_f32_min_max rect, position2_f32 point) {
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

enum rect_f32_intersection_segment_result {
    rect_f32_intersection_segment_result_0 = 0,
    rect_f32_intersection_segment_result_1 = 1 << 0,
    rect_f32_intersection_segment_result_2 = 1 << 1,
    rect_f32_intersection_segment_result_both_inside = 1 << 2,
    rect_f32_intersection_segment_result_both_outside = 1 << 3,
};

static size_t cohen_sutherland_line_clip(const rect_f32_min_max rect, position2_f32 a, position2_f32 b, position2_f32 out_clipped[2]) {
    // outcode outcode_a = rect_f32_min_max_outcode(rect, a);
    // outcode outcode_b = rect_f32_min_max_outcode(rect, b);

    // if (!(outcode_a | outcode_b)) {
    //     return 2;
    // } else if (outcode_a & outcode_b) {
    //     return 0;
    // }
}

size_t rect_f32_intersects_segment(const rect_f32 rect, const position2_f32 a, const position2_f32 b, position2_f32 out_intersections[2]) {
    return size_t();
}