#include "WeaponRataBasurera.h"
#include "../../game/Game.h"
#include "../../game/GameScene.h"
#include "../../sdlutils/RandomNumberGenerator.h"

WeaponRataBasurera::WeaponRataBasurera() : Weapon(0, 5000, 0.0f, 0.0f, "", 0.0f, 0.0f) { }

WeaponRataBasurera::~WeaponRataBasurera() {}

void
WeaponRataBasurera::callback(Vector2D shootPos, Vector2D shootDir) {

	int _randomNum = sdlutils().rand().nextInt(0, 10);

	//Genera SarnoRata si _randomNum está entre 0-4 (50% chance)
	if (_randomNum < 5) static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_sarno_rata(shootPos);
	//Genera PlimPlim si _randomNum está entre 5-7 (30% chance)
	else if (_randomNum < 8) static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_plim_plim(shootPos);
	//Genera Boom si _randomNum está entre 8-9 (20% chance)
	else static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_boom(shootPos);
}