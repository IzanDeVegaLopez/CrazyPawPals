#pragma once
#include "../../../ecs/Component.h"
#include "../../../rendering/camera.hpp"
class Transform;
class StopOnBorder: public ecs::Component {
	Transform* _tr;
	const camera_screen& _cam;
	float _halfWidth;
	float _halfHeight;
public:
	__CMPID_DECL__(ecs::cmp::STOPONBORDER);
	StopOnBorder(const camera_screen& camera, float w, float h);
	virtual ~StopOnBorder();
	void initComponent() override;
	void update(uint32_t delta_time) override;
};

