#pragma once
#include "Weapon.h"

class Revolver : public Weapon {
public:
	__CMPID_DECL__(ecs::cmp::REVOLVER);
	Revolver(int d, float cd, float dis, float speed);
	virtual ~Revolver();
	void callback(Vector2D shootPos, Vector2D shootDir) override;
};