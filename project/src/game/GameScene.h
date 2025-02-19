#pragma once

#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene() {};
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	//Methods of entities to spawn
	void spawnPlayer();
	void spawnEnemies();
	
};