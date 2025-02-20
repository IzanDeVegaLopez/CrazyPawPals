#pragma once

#include "Scene.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	~MainMenuScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	//Methods of entities to spawn
	//spawnStartButton();
};