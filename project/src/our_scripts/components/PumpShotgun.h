#pragma once
#include "Weapon.h"
class PumpShotgun : public Weapon {
protected:
	void callback(Vector2D shootPos, Vector2D shootDir) override;
	bool _has_mill;
public:
	__CMPID_DECL__(ecs::cmp::WEAPON);
	PumpShotgun();
	virtual ~PumpShotgun();

};