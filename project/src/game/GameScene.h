#pragma once

#include "Scene.h"
class Weapon;
class Transform;
class GameScene : public Scene
{
	[[maybe_unused]]

	ecs::entity_t create_enemy(Transform* tr, const std::string& spriteKey, Weapon* weapon, float health, float width, float height);
public:
	GameScene();
	~GameScene() {};
	void initScene() override;
	void enterScene() override;
	void exitScene() override;

	//Methods of entities to spawn
	ecs::entity_t create_player();

	void spawn_sarno_rata(Vector2D posVec);
	void spawn_michi_mafioso(Vector2D posVec);
	void spawn_plim_plim(Vector2D posVec);
	void spawn_boom(Vector2D posVec);
	void spawn_ratatouille(Vector2D posVec);
	void spawn_wave_manager();
	void spawn_fog();

	void generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid);
	void check_collision();
};