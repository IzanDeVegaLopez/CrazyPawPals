#pragma once
#include "Scene.h"
#include <string>
#include <list>
#include <unordered_set>

class Texture;
class Button;
class Card;
class ImageForButton;
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

	ImageForButton* _lr;
	bool _selected;

	//method to select a card randomly
	std::string select_card(GameStructs::CardType ct);
	std::string get_unique_card(GameStructs::CardType& ct, std::unordered_set<std::string>& appeared_cards);

	//methods to create reward buttons
	void create_reward_card_button(const GameStructs::ButtonProperties& bp);
	void create_reward_health_button(const GameStructs::ButtonProperties& bp);
	void create_reward_buttons();
	void refresh_rewards();
	//method to create next round button
	void create_next_round_button() {};

	//method to change the position between health button and a card button
	void change_pos(bool enter);
	void resize(ImageForButton* im, float factor);

	void create_reward_selected_button(const GameStructs::ButtonProperties& bp);
};