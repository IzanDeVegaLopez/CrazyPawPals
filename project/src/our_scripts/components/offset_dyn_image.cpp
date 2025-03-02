#include "../../game/Game.h"
#include "../../ecs/Manager.h"

#include "Transform.h"
#include "Image.h"
#include "offset_dyn_image.hpp"


void offset_dyn_image::render() {
    const SDL_Rect destination = SDL_Rect_screen_rect_from_global({
        .position = {
            .x = offset.x + transform.getPos().getX() + output_rect.rect.position.x,
            .y = offset.y + transform.getPos().getY() + output_rect.rect.position.y,
        },
        .size = output_rect.rect.size
    }, camera);

	texture.render(SDL_Rect{
        .x = int(subrect.position.x * float(texture.width())),
        .y = int(subrect.position.y * float(texture.height())),
        .w = int(subrect.size.x * float(texture.width())),
        .h = int(subrect.size.y * float(texture.height()))
    }, destination, transform.getRot());
}

