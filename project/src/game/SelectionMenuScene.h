#pragma once
#include "Scene.h"

class Texture;
class SelectionMenuScene : public Scene
{
public:
	SelectionMenuScene();
	virtual ~SelectionMenuScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	void render() override;

private:
	Texture* _choose_weapon_text;
	void create_weapon_button(GameStructs::WeaponType wt, const GameStructs::ButtonProperties& bp);
};