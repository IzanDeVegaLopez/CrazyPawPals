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

#include "../our_scripts/components/ShootComponent.h"
#include "../our_scripts/components/SimpleMove.h"

#include "../our_scripts//components/ShootComponent.h"
#include "../our_scripts//components/SimpleMove.h"
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/Bullet.h"
#include "../our_scripts/Player.h"

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

	/*Deck deck = Deck(std::list<Card*>{new Card("1"), new Card("2"), new Card("3"), new Card("4")});
	//cout << deck << endl;
	deck.add_card_to_deck(new Fireball());
	deck.add_card_to_deck(new Minigun());

	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.use_card();
	deck.reload();

	//deck.addCardToDeck(new Card("5"));
	cout << deck << endl;*/
}

void GameScene::enterScene()
{
}

void GameScene::exitScene()
{
}

void GameScene::update()
{
	Game::Instance()->get_mngr()->update(ecs::grp::GAMESCENE);
}

void GameScene::render()
{
	Game::Instance()->get_mngr()->render(ecs::grp::GAMESCENE);
}

void GameScene::spawnPlayer()
{
	auto _entity = Game::Instance()->get_mngr()->addEntity(ecs::grp::GAMESCENE);
	auto _tr = Game::Instance()->get_mngr()->getComponent<Transform>(_entity);
	_tr->init({ sdlutils().width() / 2.0f, sdlutils().height() / 2.0f }, { 0.0f,0.0f }, { 0.0f,0.0f }, s, s, 0.0f, 2.0f);
	Game::Instance()->get_mngr()->addComponent<Image>(_entity, &sdlutils().images().at("player"));
	Game::Instance()->get_mngr()->addComponent<KeyboardPlayerCtrl>(_entity);
	Game::Instance()->get_mngr()->addComponent<MovementController>(_entity);
}

void GameScene::spawnEnemies()
{
}
