#ifndef DYN_IMAGE_HPP
#define DYN_IMAGE_HPP

#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"
#include "../../rendering/camera.hpp"

class Transform;
class Texture;

struct dyn_image : public ecs::Component {
	__CMPID_DECL__(ecs::cmp::CAMERA);
	rect_f32 subrect;
	size2_f32 size;
	camera_screen &camera;
	Texture &texture;
	Transform *transform;

	dyn_image(const rect_f32 subrect, const size2_f32 size, camera_screen &camera, Texture &texture);

    virtual void initComponent() override; 
	virtual void render() override;
};

#endif