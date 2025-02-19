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
	auto _entity = Game::Instance()->get_mngr()->addEntity(ecs::scene::GAMESCENE);
	auto _tr = Game::Instance()->get_mngr()->addComponent<Transform>(_entity);
	float s = 100.0f;
	_tr->init({ sdlutils().width() / 2.0f, sdlutils().height() / 2.0f }, { 0.0f,0.0f }, s, s, 0.0f, 2.0f);

	Game::Instance()->get_mngr()->addComponent<Image>(_entity, &sdlutils().images().at("player"));
	Game::Instance()->get_mngr()->addComponent<Revolver>(_entity);
	Game::Instance()->get_mngr()->addComponent<ShootComponent>(_entity);
	Game::Instance()->get_mngr()->addComponent<KeyboardPlayerCtrl>(_entity);
	Game::Instance()->get_mngr()->addComponent<MovementController>(_entity);
}

void GameScene::spawnEnemies()
{
}

void GameScene::generate_proyectile(GameStructs::BulletProperties& bp, ecs::grpId_t gid)
{
	ecs::entity_t new_bullet = Game::Instance()->get_mngr()->addEntity(gid);

	Game::Instance()->get_mngr()->addComponent<Transform>(new_bullet, bp.init_pos, (bp.dir - bp.init_pos).normalize(), bp.width, bp.height, bp.dir.angle(bp.init_pos), bp.speed);
	Game::Instance()->get_mngr()->addComponent<Image>(new_bullet, &sdlutils().images().at("bullet_1"));
	Game::Instance()->get_mngr()->addComponent<SimpleMove>(new_bullet);
	Game::Instance()->get_mngr()->addComponent<LifetimeTimer>(new_bullet, bp.life_time);

}