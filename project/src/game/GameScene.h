#pragma once

#include "Scene.h"
class GameScene : public Scene
{
	ecs::entity_t _player;
public:
	GameScene();
	~GameScene() {};
	void initScene() override;
	void enterScene() override;
	void exitScene() override;

	//Methods of entities to spawn
	ecs::entity_t spawnPlayer();

	void spawnSarnoRata(Vector2D posVec);
	void spawnMichiMafioso(Vector2D posVec);
	void spawnPlimPlim(Vector2D posVec);
	void spawnBoom(Vector2D posVec);
	void spawnWaveManager();

	void generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid);
	void check_collision();
};