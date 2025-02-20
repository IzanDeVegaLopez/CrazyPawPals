#include "Weapon.h"
//Weapon::Weapon() : _damage(0), _cooldown(0.0f), _distance(0.0f), _speed(0.0f), _tex(""), _attack_width(50), _attack_height(50) {}

Weapon::Weapon(int d, float cd, float dis, float s, const std::string& t)
	: _damage(d), _cooldown(cd), _distance(dis), _speed(s), _tex(t), _attack_width(50), _attack_height(50)
{
}

Weapon::~Weapon() {}
/*
void 
Weapon::init(int d, float cd, float dis, float s, const std::string& t) {
	_damage = d;
	_cooldown = cd;
	_distance = dis;
	_speed = s;
	_tex = t;
}
*/
void 
Weapon::set_attack_size(int w, int h) {
	_attack_height = h; 
	_attack_width = w;
}

