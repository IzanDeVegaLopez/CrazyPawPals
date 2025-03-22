#pragma once

#include "../../../ecs/Component.h"
#include "../../../rendering/rect.hpp"
#include "../../../sdlutils/Texture.h"
#include "../../../rendering/camera.hpp"
#include "../rendering/camera_component.hpp"
class Texture;
struct camera_screen;
#include <iostream>
struct transformless_dyn_image : public ecs::Component {

	__CMPID_DECL__(ecs::cmp::TRANSFORMLESS_DYN_IMAGE);
	Texture* texture;
	//takes 0 to 1 values on viewport
	rect_f32 destination_rect;
	const camera_screen& my_camera_screen;
	float my_rotation;
	bool active = true;
	float _original_w;
	float _original_h;

	transformless_dyn_image(
		const rect_f32 subrect,
		float rotation,
		const camera_screen& camera,
		Texture* texture
	) : destination_rect(subrect), my_camera_screen(camera), texture(texture), my_rotation(rotation), _original_w(subrect.size.x), _original_h(subrect.size.y)
	{
	}

	inline SDL_Rect get_destination_rect() const {
		rect_f32 rect = rect_f32_screen_rect_from_viewport(destination_rect, my_camera_screen.screen);
		SDL_Rect destination = { rect.position.x,rect.position.y,rect.size.x,rect.size.y };
		return destination;
	};

	inline virtual void render() override {
		if (!active) return;

		rect_f32 rect = rect_f32_screen_rect_from_viewport(destination_rect, my_camera_screen.screen);
		const SDL_Rect destination = {
			int(rect.position.x),
			int(rect.position.y),
			int(rect.size.x),
			int(rect.size.y)
		}; //SDL_Rect_screen_rect_from_global(destination_rect, my_camera_screen);
		const SDL_Rect source = { 0, 0, texture->width(), texture->height() };
		texture->render(source, destination, my_rotation, nullptr);
	}
	inline void set_texture(Texture* t) { texture = t; };

	void set_active(bool v) {
		active = v;
	}
	inline void apply_filter(int r, int g, int b) { SDL_SetTextureColorMod(&texture->get_texture(), r, g, b); };

};
