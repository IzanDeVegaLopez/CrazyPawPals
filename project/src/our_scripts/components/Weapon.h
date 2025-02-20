#pragma once
//this is the abstract class of player's weapons
#include "../../ecs/Component.h"
#include <vector>
#include <string>
#include "../../utils/Vector2D.h"

class Bullet;
class Weapon : public ecs::Component {
protected:
	int _damage; //weapon's damage
	float _cooldown; //cooldown
	float _distance; //the distance to reach
	float _speed;
	std::string _tex;
	int _attack_width;
	int _attack_height;
public:
	__CMPID_DECL__(ecs::cmp::WEAPON);
	Weapon(int d, float cd, float dis, float s, const std::string& t);
	virtual ~Weapon();

	//NECCESARY, ALL TYPE OF WEAPON NEED TO CALL THIS METHOD TO INITIALIZE ATTRIBUTES
	//void init(int d, float cd, float dis, float s, const std::string& t);
	//not neccesary (50x50 as default value)
	void set_attack_size(int w, int h);
	virtual void callback(Vector2D shootPos, Vector2D shootDir) = 0; //when you attacks, this callback will be called
	inline int damage() { return _damage; };
	inline float cooldown() { return _cooldown; };
	inline float distance() { return _distance; };
};