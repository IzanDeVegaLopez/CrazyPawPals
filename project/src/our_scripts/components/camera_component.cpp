#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "camera_component.hpp"

void camera_component::update(uint32_t delta_time) {
    (void)delta_time;
    cam.screen.pixel_size = size2_i32{
        sdlutils().width(),
        sdlutils().height()
    };
}

void camera_follow::initComponent() {
    cam = Game::Instance()->get_mngr()->getComponent<camera_component>(_ent);
    assert(cam != nullptr);
}

#include <iostream>
#include <cmath>
#include <cassert>
void camera_follow::update(uint32_t delta_time) {
    const position2_i32 mouse_position = {
        int32_t(ih().getMousePos().getX()),
        int32_t(ih().getMousePos().getY()),
    };
    const position2_f32 mouse_world_position = rect_f32_global_from_screen_rect({
        .position = {
            .x = float(mouse_position.x),
            .y = float(mouse_position.y),
        },
        .size = {
            .x = 1.0f,
            .y = 1.0f,
        }
    }, cam->cam).position;

    auto &&self_target = const_cast<Transform &>(target);
    const position2_f32 target_position = {
        .x = self_target.getPos().getX(),
        .y = self_target.getPos().getY(),
    };

    camera_follow_target targets[] = {
        camera_follow_target{
            .position = target_position,
            .weight = 0.9f
        },
        {
            .position = mouse_world_position,
            .weight = 0.1f
        }
    };
    cam->cam.camera = camera_update_from_follow_multiple(
        cam->cam.camera,
        descriptor,
        targets,
        2,
        float(delta_time) / 1000.0f
    );
}
