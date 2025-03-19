#pragma once
#include "Weapon.h"

class WeaponCatKuza : public Weapon {
protected:
	void callback(Vector2D shootPos, Vector2D shootDir) override;
public:
	__CMPID_DECL__(ecs::cmp::WEAPON);
	WeaponCatKuza();
	virtual ~WeaponCatKuza();
	void wind_attack(Vector2D shootPos, Vector2D shootDir);
	void dash_attack(Vector2D shootPos, Vector2D shootDir);

private:
	void callback_3_wind(GameStructs::BulletProperties &bp);
	void callback_area(GameStructs::BulletProperties &bp);
	void callback_dash(GameStructs::BulletProperties &bp);
	int _state;
	int _wind_p;
	int _area_p;
	int _dash_p;
};