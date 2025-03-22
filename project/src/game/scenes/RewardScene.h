#pragma once
#include "Scene.h"
#include <string>
#include <list>

class Texture;
class Button;
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
	void create_reward_button(const GameStructs::ButtonProperties& bp);
	void create_reward_buttons();

	//methods to show your actual deck info
	void create_my_deck_cards();
	void create_a_deck_card(const GameStructs::ButtonProperties& bp);
	void refresh_my_deck_cards(const std::list<std::string>& cl);

	Button* _selected_card;
};