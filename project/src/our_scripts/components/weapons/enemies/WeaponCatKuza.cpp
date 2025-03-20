#include "WeaponCatKuza.h"
#include "../../../../game/Game.h"
#include "../../../../game/scenes/GameScene.h"

WeaponCatKuza::WeaponCatKuza() : Weapon(4, 5000, 20.0f, 0.1f, "p_plimplim", 1.0f, 1.0f), _state(0), _wind_p(3), _area_p(6), _dash_p(6) { }

WeaponCatKuza::~WeaponCatKuza() {}

void 
WeaponCatKuza::callback(Vector2D shootPos, Vector2D shootDir) {
	// GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	// bp.dir = shootDir;
	// bp.init_pos = shootPos;

	// callback_3_wind(bp);

	// // switch (expression)
	// // {
	// // case constant expression:
	
	// // 	break;
	
	// // default:
	// // 	break;
	// // }


	// float totalAngle = 60.0f;

	 //patrons::ShotgunPatron(bp, ecs::grp::BULLET, totalAngle, _state);

}

void WeaponCatKuza::wind_attack(Vector2D shootPos, Vector2D shootDir) {
    GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = shootDir;
	bp.init_pos = shootPos;

    callback_3_wind(bp);
}

void WeaponCatKuza::dash_attack(Vector2D shootPos, Vector2D shootDir) {
    GameStructs::BulletProperties bp = GameStructs::BulletProperties();
    bp.dir = shootDir;
	bp.init_pos = shootPos;
	
    callback_dash(bp);

}


void WeaponCatKuza::callback_3_wind(GameStructs::BulletProperties &bp){
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width* 0.5f;
	bp.height = _attack_height * 0.5f;

	bp.sprite_key = "p_plimplim";

	float totalAngle = 60.0f;

	//patrons::ShotgunPatron(bp, ecs::grp::BULLET, totalAngle, _wind_p);
}

void WeaponCatKuza::callback_area(GameStructs::BulletProperties &bp){
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width* 0.5f;
	bp.height = _attack_height * 0.5f;

	bp.sprite_key = "p_plimplim";
}

void WeaponCatKuza::callback_dash(GameStructs::BulletProperties &bp){
	bp.speed =_speed;
	bp.damage = _damage;
	bp.life_time = 2;
	bp.width = _attack_width* 0.5f;
	bp.height = _attack_height * 0.5f;

	bp.sprite_key = "p_plimplim";

	float totalAngle = 60.0f;

	//patrons::ShotgunPatron(bp, ecs::grp::BULLET, totalAngle, _state);
}