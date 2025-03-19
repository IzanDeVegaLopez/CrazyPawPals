#pragma once
#include "Weapon.h"
class Transform;
class WeaponSuperMichiMafioso : public Weapon {
protected:
	void callback(Vector2D shootPos, Vector2D shootDir) override;

public:
	enum Pattern {
		ATTACK1,
		ATTACK2,
		ATTACK3,
		SPAWN_MICHI_MAFIOSO
	};
	__CMPID_DECL__(ecs::cmp::WEAPON);
	WeaponSuperMichiMafioso(Transform* playerTr);
	virtual ~WeaponSuperMichiMafioso();
	void setAttackPattern(Pattern p);
private:
	void attack1(Vector2D shootDir);
	void attack2(Vector2D shootPos, Vector2D shootDir);
	void attack3(Vector2D shootPos, Vector2D shootDir);

	void create_area(Vector2D shootPos, Vector2D shootDir, const std::string& key_name, int damage, float speed, float life_t, float scale = 1);
	void generate_michi_mafioso();

	Pattern _currentPattern;
	Transform* _player_tr;

	Vector2D _last_shootPos;
	bool _warning;
};