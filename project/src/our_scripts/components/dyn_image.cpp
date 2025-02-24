#include "../../game/Game.h"
#include "../../ecs/Manager.h"

#include "Transform.h"
#include "Image.h"
#include "dyn_image.hpp"

dyn_image::dyn_image(const rect_f32 subrect, const size2_f32 size, camera_screen &camera, Texture &texture)
    : subrect(subrect), size(size), camera(camera), texture(texture), transform(nullptr) {
    
}

void dyn_image::initComponent()
{
    auto &&manager = *Game::Instance()->get_mngr();
    
    if (transform == nullptr) {
        transform = manager.getComponent<Transform>(_ent);
        assert(transform != nullptr);
    }
}

void dyn_image::render() {
    const SDL_Rect destination = SDL_Rect_screen_rect_from_global({
        .position = {
            .x = transform->getPos().getX(),
            .y = transform->getPos().getY()
        },
        .size = size
    }, camera
    );
    
	texture.render(SDL_Rect{
        .x = int(subrect.position.x * float(texture.width())),
        .y = int(subrect.position.y * float(texture.height())),
        .w = int(subrect.size.x * float(texture.width())),
        .h = int(subrect.size.y * float(texture.height()))
    }, destination);
}

