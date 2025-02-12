#pragma once

#include "PhysicsComponent.h"
#include "../utils/Vector2D.h"

class MovementController : public PhysicsComponent
{
public:
	MovementController();
	virtual ~MovementController();
	void update(Container* o) override;
	void accelerate(Container* o);

private:
	float _maxSpeed;
	float _reduceSpeed;

};