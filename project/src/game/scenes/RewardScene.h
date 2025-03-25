#pragma once
#include "Scene.h"
#include <string>
#include <list>

class Texture;
class Button;
class Card;
class ImagenForButton;
using LastReward = ImagenForButton*;
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
    ecs::entity_t create_card_button(const GameStructs::CardButtonProperties& bp);
	//methods to show your actual deck info
	void create_my_deck_cards();
	void create_a_deck_card(const GameStructs::CardButtonProperties& bp);
	void refresh_my_deck_cards(const std::list<std::string>& cl);

	//deck cards part
	Button* _selected_button;
	Card* _selected_card;
	Texture* _reward_bg;

	//reward part
	bool _health;
	bool _card;
	bool _object;
	bool _upgrade;

	LastReward _lr;

	//method to select a card randomly
	std::string select_card(GameStructs::CardType ct);

	//methods to create reward buttons
	void create_reward_card_button(const GameStructs::ButtonProperties& bp);
	void create_reward_health_button(const GameStructs::ButtonProperties& bp);
	void create_reward_buttons();

	//method to create next round button
	void create_next_round_button() {};
};