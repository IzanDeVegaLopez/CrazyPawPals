#pragma once

#include "Scene.h"
class GameScene : public Scene
{
	[[maybe_unused]]
	ecs::entity_t _player;
public:
	GameScene();
	~GameScene() {};
	void initScene() override;
	void enterScene() override;
	void exitScene() override;

	//Methods of entities to spawn
	ecs::entity_t spawnPlayer();

	void spawn_sarno_rata(Vector2D posVec);
	void spawn_michi_mafioso(Vector2D posVec);
	void spawn_plim_plim(Vector2D posVec);
	void spawn_boom(Vector2D posVec);
	void spawn_wave_manager();

	void generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid);
	void check_collision();
};