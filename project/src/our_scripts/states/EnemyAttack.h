#pragma once
#include "PhysicsComponent.h"

class EnemyAttack :public PhysicsComponent
{
public:
	EnemyAttack(Container*, float);
	virtual ~EnemyAttack();
	void update(Container* o) override;
private:
	Container* _target;
	float _wantedDistance;
};

