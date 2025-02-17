#pragma once
//this is the abstract class of player's weapons
#include "../../ecs/Component.h"

class Bullet;
class Weapon : public ecs::Component {
protected:
	int _damage; //weapon's damage
	float _cooldown; //cooldown
	float _distance; //the distance to reach
	float _speed;
	std::vector<Bullet*> _pool; //attacks pool
public:
	__CMPID_DECL__(ecs::cmp::WEAPON);
	Weapon(int d, float cd, float dis, float speed);
	virtual ~Weapon();
	virtual void callback(Vector2D shootPos, Vector2D shootDir) = 0; //when you attacks, this callback will be called
	inline int damage() { return _damage; };
	inline float cooldown() { return _cooldown; };
	inline float distance() { return _distance; };
};