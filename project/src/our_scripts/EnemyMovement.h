#pragma once
#include "PhysicsComponent.h"

class EnemyMovement:public PhysicsComponent
{
public: 
	EnemyMovement(Container*);
	virtual ~EnemyMovement();
	void update(Container* o) override;

private:
	Container* _target;
};

