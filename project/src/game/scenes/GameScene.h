#pragma once

#include "Scene.h"
#include <functional>
class Weapon;
class Transform;
class StateMachine;

class GameScene : public event_system::event_receiver, public Scene
{
	[[maybe_unused]]

	ecs::entity_t create_enemy(Transform* tr, const std::string& spriteKey, Weapon* weapon, float health, float width, float height);

	void add_transition(StateMachine& state, const std::string& from, const std::string& to, const std::function<bool()>& condition);
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
	void spawn_catkuza(Vector2D posVec);
	void spawn_ratatouille(Vector2D posVec);
	void spawn_wave_manager();
	void spawn_fog();
	void spawn_super_michi_mafioso(Vector2D posVec);

	void generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid);
	void check_collision();
	void event_callback0(const event_system::event_receiver::Msg& m) override;
protected:
	float deccel_spawned_creatures_multi = 1;
};