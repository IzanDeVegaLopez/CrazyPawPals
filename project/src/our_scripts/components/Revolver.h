#pragma once
#include "Weapon.h"

class Revolver : public Weapon {
public:
	__CMPID_DECL__(ecs::cmp::WEAPON);
	Revolver();
	virtual ~Revolver();
	void callback(Vector2D shootPos, Vector2D shootDir) override;
};