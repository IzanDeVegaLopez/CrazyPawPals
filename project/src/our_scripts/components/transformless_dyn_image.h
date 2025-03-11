#pragma once

#include "../../ecs/Component.h"
#include "../../rendering/rect.hpp"
#include "../../sdlutils/Texture.h"
#include "../../rendering/camera.hpp"
#include "../components/camera_component.hpp"
class Texture;
class camera_screen;


struct transformless_dyn_image : public ecs::Component {

	__CMPID_DECL__(ecs::cmp::TRANSFORMLESS_DYN_IMAGE);
	Texture& texture;
	rect_f32 destination_rect;
	rect_f32 source_subrect;
	const camera_screen& my_camera_screen;
	//const rect_component& output_rect;
	float my_rotation;


	transformless_dyn_image(
		const rect_f32 subrect,
		const rect_f32 offset,
		//const rect_component& output_rect,
		float rotation,
		const camera_screen& camera,
		Texture& texture
	) : destination_rect(subrect), source_subrect(offset), my_camera_screen(camera), texture(texture), my_rotation(rotation)
	{
	}



	inline virtual void render() override {
		//CAMERA THINGIES
		camera_screen cam_screen = camera_screen();
		std::pair<int, int> position = { sdlutils().width(), sdlutils().height() };
		//camera position, similar to aspect ratio but in world units (suppose player is 1 world unit, how many players will fit on camera kinda)
		cam_screen.camera = { {0,0},{8,6} };
		//camera screen on pixels size
		cam_screen.screen = { position.first, position.second };
		

		//Texture& card_texture = sdlutils().images().at(descriptor.card_image_key.data());
		const SDL_Rect destination = SDL_Rect_screen_rect_from_global(destination_rect, my_camera_screen);
		const SDL_Rect source = { 0, 0, texture.width(), texture.height() };

		//card_texture.render(source, destination, angle, nullptr, flip);
		const SDL_Rect subsource = {
			int(source_subrect.position.x * source.w),
			int(source_subrect.position.y * source.h),
			int(source_subrect.size.x * source.w),
			int(source_subrect.size.y * source.h)
		};
		//card_texture.render(subsource, destination, my_rotation, nullptr, flip);
	//}
		texture.render(subsource, destination, my_rotation, nullptr);
	}

};
