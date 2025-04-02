#include "RampCanon.h"
#include "../../../../game/Game.h"
#include "../../../../game/scenes/GameScene.h"
#include "our_scripts/card_system/ShootPatrons.hpp"
#include "../../cards/Mana.h"

#include <iostream>

RampCanon::RampCanon() : Weapon(3, 750.0f, 0.5f, 0.1f, "p_rampage", 1.0f, 1.0f){ 
	auto player = Game::Instance()->get_mngr()->getHandler(ecs::hdlr::PLAYER);
	_mana = Game::Instance()->get_mngr()->getComponent<ManaComponent>(player);
	assert(_mana != nullptr);
}

RampCanon::~RampCanon() {}

void
RampCanon::callback(Vector2D shootPos, Vector2D shootDir) {

	if (_mana->mana_count() >= 0.5) { // Check if the player has more than 0.5 mana to shoot

		_mana->change_mana(-1);// Subtract the mana cost

		GameStructs::BulletProperties bp = GameStructs::BulletProperties();
		bp.dir = shootDir;
		bp.init_pos = shootPos;
		bp.speed = _speed;
		bp.damage = _damage;
		bp.life_time = _distance;
		bp.width = _attack_width * 0.6f;
		bp.height = _attack_height * 0.6f;
		bp.sprite_key = _tex;
		bp.weapon_type = GameStructs::RAMP_CANON;// weapon type for collision
		bp.collision_filter = GameStructs::collide_with::enemy;

		static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::BULLET);
	}
	//else std::cout << "No hay suficiente mana para disparar" << std::endl;
	
}