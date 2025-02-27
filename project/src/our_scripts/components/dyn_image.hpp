#ifndef DYN_IMAGE_HPP
#define DYN_IMAGE_HPP

#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"
#include "../../rendering/camera.hpp"

class Transform;
class Texture;
struct rect_component;

struct dyn_image : public ecs::Component {
	__CMPID_DECL__(ecs::cmp::CAMERA);
	rect_f32 subrect;
	const rect_component &output_rect;
	const camera_screen &camera;
	Texture &texture;
	const Transform &transform;

	//static_assert(false);
	inline dyn_image(
		const rect_f32 subrect,
		const rect_component &output_rect,
		const camera_screen &camera,
		Texture &texture,
		const Transform &transform
	) : subrect(subrect), output_rect(output_rect), camera(camera), texture(texture), transform(transform) {};

    virtual void render() override;
};

#endif