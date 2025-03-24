#pragma once
#include "Scene.h"

class Texture;
class Card;
class ImageForButton;
using LastButton = ImageForButton*;
class RewardScene : public Scene
{
public:
	RewardScene();
	virtual ~RewardScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	void render() override;

private:

	ecs::Manager* _mngr;

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
	std::vector<ImageForButton*> _cards_vector[3];
	//Selects 3 cards randomly 
	std::string select_cards(int i);

	//buttons
	void create_reward_button(const GameStructs::ButtonProperties& bp);
	void create_next_round_button(const GameStructs::ButtonProperties& bp);

	//What to do with each reward
	void card_reward();
	//void health_reward();
	//void object_reward(); //No objects yet
	//void upgrade_reward();
};