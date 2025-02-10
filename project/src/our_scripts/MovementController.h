#pragma once

#include "PhysicsComponent.h"
#include "KeyboardPlayerCtrl.h"
#include "../utils/Vector2D.h"

class MovementController : public PhysicsComponent
{
public:
	MovementController();
	virtual ~MovementController();
	void update(Container* o) override;

private:
	float _maxSpeed;
	float _reduceSpeed;
	Vector2D _direction;
};