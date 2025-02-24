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
	void spawnSarnoRata(Vector2D posVec);
	void spawnMichiMafioso(Vector2D posVec);
	void spawnPlimPlim(Vector2D posVec);
	void spawnBoom(Vector2D posVec);
	void spawnWaveManager();
	
	void generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid, const std::string& texName);
	
};