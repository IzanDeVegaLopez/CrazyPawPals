#include "GameScene.h"

#include "Game.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/Transform.h"
#include "../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../our_scripts/components/MovementController.h"
#include "../our_scripts/components/LifetimeTimer.h"

#include "../our_scripts/components/Revolver.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/components/WaveManager.h"
#include "../our_scripts/components/EnemyStatemachine.h"

#include <iostream>
#include <string>

GameScene::GameScene()
{

}

void GameScene::initScene()
{
#pragma region Bullets
	//std::vector<Bullet*> b;

#pragma endregion

#pragma region Enemies
	spawnWaveManager();

#pragma endregion

#pragma region Player

	//spawnPlayer();

#pragma endregion Deck

	//Deck deck = Deck(std::list<Card*>{new Card("1"), new Card("2"), new Card("3"), new Card("4")});
	////cout << deck << endl;
	//deck.add_card_to_deck(new Fireball());
	//deck.add_card_to_deck(new Minigun());

	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.reload();

	////deck.addCardToDeck(new Card("5"));
	//cout << deck << endl;*/
}

void GameScene::enterScene()
{
}

void GameScene::exitScene()
{
}

void GameScene::update(uint32_t delta_time)
{
	Game::Instance()->get_mngr()->update(ecs::scene::GAMESCENE,delta_time);
}

void GameScene::render()
{
	Game::Instance()->get_mngr()->render(ecs::scene::GAMESCENE);
}

void GameScene::spawnPlayer()
{
	auto* revolver = new Revolver();
	std::list<Card*> c = { new Fireball(), new Minigun(), new Fireball(), new Minigun() };
	create_entity(
		ecs::grp::PLAYER,
		ecs::scene::GAMESCENE,
		new Transform({ sdlutils().width() / 2.0f, sdlutils().height() / 2.0f }, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("player")),
		revolver,
		new Mana(),
		new Deck(c),
		new MovementController(),
		new KeyboardPlayerCtrl()
		);
	revolver->initComponent();
	revolver->set_attack_size(50, 20);
}

void GameScene::spawnSarnoRata(Vector2D posVec)
{
	create_entity(
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),
		new MovementController()
	);
}

void GameScene::spawnMichiMafioso(Vector2D posVec)
{
	create_entity(
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),
		new MovementController()
	);
}

void GameScene::spawnPlimPlim(Vector2D posVec)
{
	create_entity(
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),
		new MovementController()
	);
}

void GameScene::spawnBoom(Vector2D posVec)
{
	create_entity(
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),
		new MovementController()
	);
}


void GameScene::spawnWaveManager()
{
	create_entity(
		new WaveManager()
	);
}

void GameScene::generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid, const std::string& texName)
{
	(void)gid;
	//std::cout << bp.speed << std::endl;
	create_entity(
		gid,
		ecs::scene::GAMESCENE,
		new Transform(bp.init_pos, bp.dir, bp.width, bp.height, bp.rot, bp.speed),
		new Image(&sdlutils().images().at(texName)),
		//new SimpleMove(),
		new LifetimeTimer(bp.life_time)
	);
}