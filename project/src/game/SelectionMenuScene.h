#pragma once
#include "Scene.h"


class SelectionMenuScene : public Scene
{
public:
	SelectionMenuScene();
	virtual ~SelectionMenuScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;

private:
	void create_weapon_button(GameStructs::WeaponType wt, const GameStructs::ButtonProperties& bp);
};