#pragma once
#include "Scene.h"
#include <vector>
#include <string>
#include <list>

class GameOverScene : public Scene
{
public:
	GameOverScene();
	virtual ~GameOverScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	void render() override;

private:
	void create_enter_button();
	//flags to control if has been selected
	bool _reward_selected;
	//last selected
	LastButton _last_selected;

	//types of reward - activate when needed
	bool _health;
	bool _card;
	bool _object;
	bool _upgrade;

	//Vector with the 3 cards
	std::vector<Card*> _cards_vector[];
	//Selects 3 cards randomly 
	void select_cards();

	//buttons
	void create_reward_button(const GameStructs::ButtonProperties& bp);
	void create_next_round_button(const GameStructs::ButtonProperties& bp);

	//What to do with each reward
	void card_reward();
	//void health_reward();
	//void object_reward(); //No objects yet
	//void upgrade_reward();
};