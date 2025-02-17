#pragma once

#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"

class Transform;
class MovementController : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::MOVEMENTCONTROLLER);
	MovementController();
	virtual ~MovementController();
	void init_component() override;
	void update(int delta_time) override;
	void accelerate();

private:
	float _maxSpeed;
	float _reduceSpeed;
	Transform* _tr;

};