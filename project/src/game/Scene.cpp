#include "Scene.h"
#include <cassert>

#include "../our_scripts/components/Transform.h"
#include "../our_scripts/components/camera_component.hpp"
ecs::entity_t Scene::rendering::create_camera(
    const ecs::sceneId_t scene,
    const camera_creation_descriptor_flags flags,
    CZPP_NULLABLE const Transform *optional_follow_target
) {
    assert(
        (flags & camera_creation_descriptor_options_follow) == (optional_follow_target != nullptr)
        && "error: follow target must be provided if and only if the follow flag is set"
    );

    auto &&manager = *Game::Instance()->get_mngr();
    auto camera = manager.addEntity(scene);
    if (flags & camera_creation_descriptor_options_set_handler) {
        manager.setHandler(ecs::hdlr::CAMERA, camera);
    }

    auto transform = manager.addComponent<Transform>(camera, Vector2D(0.0, 0.0), Vector2D(0.0, 0.0), 0.0, 0.0);
    assert(transform != nullptr && "error: failed to add camera transform");

    constexpr static const ::camera default_camera{
        .position = {0.0, 0.0},
        .half_size = {8.0, 4.5},
    };
    auto &&device = *manager.addComponent<camera_component>(camera, camera_screen{
		.camera = default_camera,
		.screen = {
			.pixel_size = {sdlutils().width(), sdlutils().height()},
		},
	});

    constexpr static const float default_lookahead_time = 1.0;
    constexpr static const float default_semi_reach_time = 2.5;
    if (flags & camera_creation_descriptor_options_follow) {
        auto follow = manager.addComponent<camera_follow>(camera, camera_follow_descriptor{
            .previous_position = device.cam.camera.position,
            .lookahead_time = default_lookahead_time,
            .semi_reach_time = default_semi_reach_time
        }, device, *optional_follow_target);
        assert(follow != nullptr && "error: failed to add camera follow");
    }

    constexpr static const ::rect_f32 default_bounds{
        .position = {0.0, 0.0},
        .size = {32.0, 18.0},
    };
    if (flags & camera_creation_descriptor_options_clamp) {
        auto clamp = manager.addComponent<camera_clamp>(camera, camera_clamp_descriptor{
            .bounds = default_bounds
        }, device);
        assert(clamp != nullptr && "error: failed to add camera clamp");
    }
    return camera;
}