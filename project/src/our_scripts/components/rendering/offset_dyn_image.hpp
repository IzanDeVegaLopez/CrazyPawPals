#ifndef OFFSET_DYN_IMAGE_HPP
#define OFFSET_DYN_IMAGE_HPP

#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../../rendering/camera.hpp"
#include "rect_component.hpp"

class Transform;
class Texture;
//struct rect_component;

struct offset_dyn_image : public ecs::Component {
	__CMPID_DECL__(ecs::cmp::OFFSET_DYN_IMAGE);
	rect_f32 subrect;
	position2_f32 offset;
	const rect_component &output_rect;
	const camera_screen &camera;
	Texture &texture;
	const Transform &transform;


	virtual void render() override;

	//static_assert(false);
	inline offset_dyn_image(
		const rect_f32 subrect,
		const position2_f32 offset,
		const rect_component &output_rect,
		const camera_screen &camera,
		Texture &texture,
		const Transform &transform
	) : subrect(subrect), offset(offset), output_rect(output_rect), camera(camera), texture(texture), transform(transform) {};
};

#endif