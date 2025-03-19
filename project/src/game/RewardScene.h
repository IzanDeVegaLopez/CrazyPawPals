#pragma once
#include "Scene.h"

class Texture;
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

	//flags to control if has been selected
	bool _reward_selected;


	void create_reward_button(const GameStructs::ButtonProperties& bp);
	void create_next_round_button(const GameStructs::ButtonProperties& bp);
};