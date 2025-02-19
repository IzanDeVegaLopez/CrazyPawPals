#include "GameScene.h"

GameScene::GameScene()
{

}

void GameScene::initScene()
{
	_mngrGame = new ecs::Manager();
#pragma region Bullets
	//std::vector<Bullet*> b;

#pragma endregion

#pragma region Player

	//auto player = new Player(_mngr);

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

void GameScene::spawnPlayer()
{
}

void GameScene::spawnEnemies()
{
}
