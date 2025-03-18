#pragma once
#include "Weapon.h"

class WeaponSuperMichiMafioso : public Weapon {
protected:
	void callback(Vector2D shootPos, Vector2D shootDir) override;

public:
	enum Pattern {
		NONE,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		SPAWN_MICHI_MAFIOSO
	};
	__CMPID_DECL__(ecs::cmp::WEAPON);
	WeaponSuperMichiMafioso();
	virtual ~WeaponSuperMichiMafioso();
	void setAttackPattern(Pattern p);

private:
	void attack1(Vector2D shootPos, Vector2D shootDir);
	void attack2(Vector2D shootPos, Vector2D shootDir);
	void attack3(Vector2D shootPos, Vector2D shootDir);
	void generate_michi_mafioso();

	Pattern _currentPattern;
};