#pragma once
#include "../../ecs/Component.h"


class BulletData :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::BULLETDATA);

	BulletData(int d): _damage(d){};
	virtual ~BulletData() {};
	inline int damage() const { return _damage; }
protected:
	int _damage;
};
