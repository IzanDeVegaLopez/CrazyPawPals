#include "card_rendering.hpp"
#include "../sdlutils/SDLUtils.h"

const card_rendering_descriptor_options_flags flip_flags =
    (card_rendering_descriptor_options_flip_horizontal | card_rendering_descriptor_options_flip_vertical);

SDL_Rect card_rendering_descriptor_render(
    const card_rendering_descriptor &descriptor,
    const camera_screen &camera_screen,
    const rect_f32 destination_rect,
    const rect_f32 source_subrect,
    const float angle,
    const card_rendering_descriptor_options_flags flags
) {
    Texture &card_texture = sdlutils().images().at(descriptor.card_image_key.data());
    const SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(flags & (flip_flags));

    const SDL_Rect destination = SDL_Rect_screen_rect_from_global(destination_rect, camera_screen);
    const SDL_Rect source = { 0, 0, card_texture.width(), card_texture.height() };

    if (flags & card_rendering_descriptor_options_full_subrect) {
        card_texture.render(source, destination, angle, nullptr, flip);
    } else {
        const SDL_Rect subsource = {
            source_subrect.position.x,
            source_subrect.position.y,
            source_subrect.size.x * source.w,
            source_subrect.size.y * source.x
        };
        card_texture.render(subsource, destination, angle, nullptr, flip);
    }

    return destination;
}

SDL_Rect card_description_rendering_descriptor_render(
    const card_description_rendering_descriptor &descriptor,
    const camera_screen &camera_screen,
    SDL_Renderer &renderer,
    const rect_f32 destination_rect,
    const SDL_Color text_color,
    const SDL_Color background_color
) {
    Texture &background = sdlutils().images().at(descriptor.background_key.data());
    SDL_SetTextureColorMod(&background.get_texture(), background_color.r, background_color.g, background_color.b);
    
    const SDL_Rect destination = SDL_Rect_screen_rect_from_global(destination_rect, camera_screen);
    SDL_SetRenderDrawColor(&renderer, background_color.r, background_color.g, background_color.b, background_color.a);
	SDL_RenderFillRect(&renderer, &destination);
    SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
    
    const rect_f32 name_rect = {
        {
            destination_rect.position.x + descriptor.name_subrect.position.x,
            destination_rect.position.y + descriptor.name_subrect.position.y,
        },
        {
            destination_rect.size.x * descriptor.name_subrect.size.x,
            destination_rect.size.y * descriptor.name_subrect.size.y,
        }
    };
    const rect_f32 description_rect = {
        {
            destination_rect.position.x + descriptor.description_subrect.position.x,
            destination_rect.position.y + descriptor.description_subrect.position.y,
        },
        {
            destination_rect.size.x * descriptor.description_subrect.size.x,
            destination_rect.size.y * descriptor.description_subrect.size.y
        }
    };

    Font &font = sdlutils().fonts().at(descriptor.font_key.data());
	Texture description{&renderer, descriptor.description.data(), font, text_color};
    description.render(SDL_Rect_screen_rect_from_global(description_rect, camera_screen));

    Texture name{&renderer, descriptor.name.data(), font, text_color};
    name.render(SDL_Rect_screen_rect_from_global(name_rect, camera_screen));
    return destination;
}
