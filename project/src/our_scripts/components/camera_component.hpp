#ifndef CAMERA_COMPONENT_HPP
#define CAMERA_COMPONENT_HPP

#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"
#include "../../rendering/camera.hpp"
#include "../../rendering/camera_follow.hpp"
#include "Transform.h"

struct camera_component : public ecs::Component {
	__CMPID_DECL__(ecs::cmp::CAMERA);
	camera_screen cam;

	inline camera_component(const camera_screen cam) : cam(cam) {}
	void update(uint32_t delta_time) override;
};

struct camera_follow : public ecs::Component {
	__CMPID_DECL__(ecs::cmp::CAMERA_FOLLOW);
	camera_follow_descriptor descriptor;
	camera_component *cam;
	const Transform &target;

	virtual void initComponent() override;

	inline camera_follow(const camera_follow_descriptor descriptor, const Transform &target) : descriptor(descriptor), target(target) {}
	void update(uint32_t delta_time) override;
};

struct camera_clamp : public ecs::Component {
	__CMPID_DECL__(ecs::cmp::CAMERA_CLAMP);
	camera_clamp_descriptor clamp;
	camera_component *cam;

	inline camera_clamp(const camera_clamp_descriptor clamp) : clamp(clamp) {}
	void update(uint32_t delta_time) override;
};

#endif