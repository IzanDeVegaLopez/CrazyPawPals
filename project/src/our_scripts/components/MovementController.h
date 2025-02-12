#pragma once

#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"

class MovementController : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::MOVEMENTCONTROLLER);
	MovementController();
	virtual ~MovementController();
	void update() override;
	void accelerate();

private:
	float _maxSpeed;
	float _reduceSpeed;

};