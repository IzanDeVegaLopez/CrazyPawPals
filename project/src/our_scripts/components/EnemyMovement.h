#pragma once
#include "../../ecs/Component.h"

class Transform;

class EnemyMovement: public ecs::Component
{
public: 
	__CMPID_DECL__(ecs::cmp::ENEMYMOVENT);

	EnemyMovement();
	virtual ~EnemyMovement();
	void initComponent() override;
	void update(uint32_t delta_time) override;


private:
	Transform* _tr;
	Transform* _playerTr;
};

