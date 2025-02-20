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
	void initComponent() override;
	void update(uint32_t delta_time) override;
	void accelerate();
	void set_input(Vector2D);

private:
	float _maxSpeed = 6.0f;
	float _acceleration = 3.5f;
	float _decceleration = 1.0f;
	Vector2D _input;
	Transform* _tr;

};