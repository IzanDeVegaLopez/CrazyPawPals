#pragma once
#include "Scene.h"
#include <string>
#include <list>
#include <unordered_set>
#include <utility>

class Texture;
class Button;
class ImageForButton;
class MythicItem;
class MythicScene : public Scene
{
public:
	MythicScene();
	virtual ~MythicScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	void update(uint32_t delta_time) override;
private:
	//Buttons on top (each one)
	ecs::entity_t create_mythic_button(const GameStructs::ButtonProperties& bp);
	//All buttons
	void create_reward_buttons();
	void refresh_mythics();

	//methods to show your actual mythic objects
	void create_my_mythic();
	//method to create the mythics i got
	void create_a_mythic(const GameStructs::ButtonProperties& bp);
	void refresh_my_mythic(const std::vector<MythicItem*> ml); //ml - Mythic List

	//General mythic part
	ImageForButton* _lr;
	bool _selected;
	bool _activate_confirm_button; //to activate button
	ecs::entity_t _chosen_mythic;

	//method to select a mythic randomly
	std::string select_mythic(GameStructs::MythicType Mt);
	std::pair<std::string, GameStructs::MythicType> get_unique_mythic(std::unordered_set<std::string>& appeared_mythic);


	//method to create button to select card
	void create_reward_selected_button(const GameStructs::ButtonProperties& bp);

	//method to create next round button
	void create_next_round_button();

	//Resize botones
	void resize(ImageForButton* im, float factor);

};