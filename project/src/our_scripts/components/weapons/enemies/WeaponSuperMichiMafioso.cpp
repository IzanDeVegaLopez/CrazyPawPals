#include "WeaponSuperMichiMafioso.h"
#include "../../../../game/Game.h"
#include "../../../../game/scenes/GameScene.h"
#include "../../movement/Transform.h"
#include <our_scripts/components/WaveManager.h>


WeaponSuperMichiMafioso::WeaponSuperMichiMafioso(Transform* playerTr) 
	: Weapon(4, 200, 20.0f, 0.1f, "p_michi_mafioso", 1.0f, 1.0f), _currentPattern(ATTACK1), _player_tr(playerTr),_warning(false){ }

WeaponSuperMichiMafioso::~WeaponSuperMichiMafioso() {}
void WeaponSuperMichiMafioso::create_area(Vector2D shootPos, Vector2D shootDir, const std::string& key_name, int damage, float speed, float life_t, float scale) {

	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = shootDir;
	bp.init_pos = shootPos;
	bp.speed = speed;
	bp.damage = damage;
	bp.life_time = life_t;
	bp.width = _attack_width * scale;
	bp.height = _attack_height * scale;
	bp.sprite_key = key_name;
	bp.collision_filter = GameStructs::collide_with::player;

	static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::ENEMYBULLETS);
}

void WeaponSuperMichiMafioso::attack1(Vector2D shootDir) {
	int scale = 2;
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = shootDir;
	bp.speed = 0;
	bp.width = _attack_width * scale;
	bp.height = _attack_height * scale;

	if (_warning) {
		bp.init_pos =_last_shootPos;
		bp.damage = 1.0f;
		bp.life_time = 1.0f;
		bp.sprite_key = "p_super_michi_mafioso";
		bp.collision_filter = GameStructs::collide_with::player;
		_warning = false;
	}
	else {
		_last_shootPos = _player_tr->getPos();
		bp.init_pos = _last_shootPos;
		bp.damage = 0;
		bp.life_time = 0.4f;
		bp.sprite_key ="attack_warning";
		bp.collision_filter = GameStructs::collide_with::none;
		_warning = true;
	}

	static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::ENEMYBULLETS);
}
void WeaponSuperMichiMafioso::attack2(Vector2D shootPos, Vector2D shootDir) {
	create_area(shootPos, shootDir, "p_super_michi_mafioso", _damage, _speed, 2.0f);
	Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::WAVE))->add_num_enemy();
}

void WeaponSuperMichiMafioso::attack3(Vector2D shootPos, Vector2D shootDir) {
	const int numAreas = 5;
	const float radius = 1.5f; 
	const float angle = 40.0f;

	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	Vector2D initialRot = bp.dir = shootDir;
	bp.speed = 0;
	bp.damage = _damage;
	bp.life_time = 3.0f;
	bp.width = radius;
	bp.height = radius;
	bp.sprite_key = "p_super_michi_mafioso";
	bp.collision_filter = GameStructs::collide_with::player;

	for (int i = 0; i < numAreas; ++i) {
		float angleOffset = (angle * (i / (std::max(((float)numAreas - 1.0f), 1.0f))) - angle / 2.0f) * (M_PI / 180.0f);
		Vector2D rotatedDir(
			initialRot.getX() * cos(angleOffset) - initialRot.getY() * sin(angleOffset),
			initialRot.getX() * sin(angleOffset) + initialRot.getY() * cos(angleOffset)
		);
		bp.init_pos = shootPos + rotatedDir * 2;
		static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::BULLET);
	}
}
void WeaponSuperMichiMafioso::generate_michi_mafioso() {
	Vector2D posRandom = _tr->getPos() + Vector2D{ (float)sdlutils().rand().nextInt(1, 5), (float)sdlutils().rand().nextInt(1, 5) };
	static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_michi_mafioso(posRandom);
}

void
WeaponSuperMichiMafioso::callback(Vector2D shootPos, Vector2D shootDir) {

	switch (_currentPattern) {
		case ATTACK1: 
			attack1(shootDir); 
			break;
		case ATTACK2: 
			attack2(shootPos, shootDir); 
			break;
		case ATTACK3: 
			attack3(shootPos, shootDir); 
			break;
		case SPAWN_MICHI_MAFIOSO: 
			generate_michi_mafioso(); 
			break;
		default:
			break;
	}
}

void WeaponSuperMichiMafioso::setAttackPattern(Pattern pattern) {
	_currentPattern = pattern;
}