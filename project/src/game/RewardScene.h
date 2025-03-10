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
	//Anadirle la textura
	void create_reward_button(const GameStructs::ButtonProperties& bp);
};