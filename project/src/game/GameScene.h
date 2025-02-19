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

	void update(uint32_t delta_time) override;
	void render() override;

	//Methods of entities to spawn
	void spawnPlayer();
	void spawnEnemies();

	void generate_proyectile(GameStructs::BulletProperties& bp, ecs::grpId_t gid);
	
};