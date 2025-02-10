#ifndef CARD_RENDERING_HPP
#define CARD_RENDERING_HPP

#include <string>
#include <SDL.h>
#include <cstdint>

struct card_rendering_descriptor {
    std::string_view card_image_key;
    uint16_t mana_cost;
};
enum card_rendering_descriptor_options {
    card_rendering_descriptor_options_none = 0,
    card_rendering_descriptor_options_flip_horizontal = 1 << 0,
    card_rendering_descriptor_options_flip_vertical = 1 << 1,
    card_rendering_descriptor_options_full_subrect = 1 << 2,
};
typedef uint8_t card_rendering_descriptor_options_flags;
extern const card_rendering_descriptor_options_flags flip_flags;

void card_rendering_descriptor_render(
    const card_rendering_descriptor &descriptor,
    const SDL_Rect destination_rect,
    const SDL_Rect source_subrect,
    const float angle,
    const card_rendering_descriptor_options_flags flags
);


struct card_description_rendering_descriptor {
    SDL_Rect name_subrect;
    SDL_Rect description_subrect;
    std::string_view background_key;
    std::string_view font_key;
    std::string_view name;
    std::string_view description;
};

void card_description_rendering_descriptor_render(
    const card_description_rendering_descriptor &descriptor,
    const SDL_Rect destination_rect,
    const SDL_Color text_color,
    const SDL_Color background_color
);

#endif