#include "WeaponRataBasurera.h"
#include "../../../../game/Game.h"
#include "../../../../game/scenes/GameScene.h"
#include "../../../../sdlutils/RandomNumberGenerator.h"

WeaponRataBasurera::WeaponRataBasurera() : Weapon(0, 5000, 0.0f, 0.0f, "", 0.0f, 0.0f) { }

WeaponRataBasurera::~WeaponRataBasurera() {}

void
WeaponRataBasurera::sendHealthComponent(Health* health) {
	_health = health;
}

void
WeaponRataBasurera::callback(Vector2D shootPos, Vector2D shootDir) {

	int _randomNum = sdlutils().rand().nextInt(0, 10);

	//Genera SarnoRata si _randomNum est� entre 0-4 (50% chance)
	if (_randomNum < 5) {
		static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_sarno_rata(shootPos);
		Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::WAVE)[0])->newEnemy();
	}
	//Genera PlimPlim si _randomNum est� entre 5-7 (30% chance)
	else if (_randomNum < 8) {
		static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_plim_plim(shootPos);
		Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::WAVE)[0])->newEnemy();
	}
	//Genera Boom si _randomNum est� entre 8-9 (20% chance)
	else {
		static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_boom(shootPos);
		Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::WAVE)[0])->newEnemy();
	}

	if (_health->getHealth() <= 0) {
		static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_rey_basurero(shootPos);
		Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getEntities(ecs::hdlr::WAVE)[0])->newEnemy();
	}
}