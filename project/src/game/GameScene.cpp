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

#include "../our_scripts/components/ShootComponent.h"
#include "../our_scripts/components/Revolver.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/Deck.hpp"

#include <iostream>

GameScene::GameScene()
{

}

void GameScene::initScene()
{
#pragma region Bullets
	//std::vector<Bullet*> b;

#pragma endregion

#pragma region Player

	spawnPlayer();

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
	//auto* revolver = new Revolver();
	//revolver->init(4, 0.5f, 20.0f, 3.0f, "revolver");
	std::list<Card*> c = { new Fireball(), new Minigun(), new Fireball(), new Minigun() };

	create_entity(
		new Transform({ sdlutils().width() / 2.0f, sdlutils().height() / 2.0f }, {0.0f,0.0f}, 100.0f,100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("player")),
		new Revolver(),
		new ShootComponent(),
		new Mana(),
		new Deck(c),
		new KeyboardPlayerCtrl(),
		new MovementController()
		);

}

void GameScene::spawnEnemies()
{
}

void GameScene::generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid)
{
	std::cout << bp.speed << std::endl;
	create_entity(
		new Transform(bp.init_pos, bp.dir, bp.width, bp.height, bp.dir.angle(bp.init_pos), bp.speed),
		new Image(&sdlutils().images().at("bullet_1")),
		//new SimpleMove(),
		new LifetimeTimer(bp.life_time)
	);
}