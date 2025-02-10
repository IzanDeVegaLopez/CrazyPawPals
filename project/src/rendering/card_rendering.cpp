#include "card_rendering.hpp"
#include "../sdlutils/SDLUtils.h"

const card_rendering_descriptor_options_flags flip_flags =
    (card_rendering_descriptor_options_flip_horizontal | card_rendering_descriptor_options_flip_vertical);

void card_rendering_descriptor_render(
    const card_rendering_descriptor &descriptor,
    const SDL_Rect destination_rect,
    const SDL_Rect source_subrect,
    const float angle,
    const card_rendering_descriptor_options_flags flags
) {
    Texture &card_texture = sdlutils().images().at(descriptor.card_image_key.data());
    const SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(flags & (flip_flags));
    if (flags & card_rendering_descriptor_options_full_subrect) {
        const SDL_Rect source = { 0, 0, card_texture.width(), card_texture.height() };
        card_texture.render(source, destination_rect, angle, nullptr, flip);
    } else {
        card_texture.render(source_subrect, destination_rect, angle, nullptr, flip);
    }
}

void card_description_rendering_descriptor_render(
    const card_description_rendering_descriptor &descriptor,
    SDL_Renderer &renderer,
    const SDL_Rect destination_rect,
    const SDL_Color text_color,
    const SDL_Color background_color
) {
    Texture &background = sdlutils().images().at(descriptor.background_key.data());
    SDL_SetTextureColorMod(&background.get_texture(), background_color.r, background_color.g, background_color.b);
    background.render(destination_rect);
    
    const SDL_Rect name_rect = {
        destination_rect.x + descriptor.name_subrect.x,
        destination_rect.y + descriptor.name_subrect.y,
        descriptor.name_subrect.w,
        descriptor.name_subrect.h
    };
    const SDL_Rect description_rect = {
        destination_rect.x + descriptor.description_subrect.x,
        destination_rect.y + descriptor.description_subrect.y,
        descriptor.description_subrect.w,
        descriptor.description_subrect.h
    };

    Font &font = sdlutils().fonts().at(descriptor.font_key.data());
	Texture description{&renderer, descriptor.description.data(), font, text_color};
    description.render(description_rect);

    Texture name{&renderer, descriptor.name.data(), font, text_color};
    name.render(name_rect);
}
