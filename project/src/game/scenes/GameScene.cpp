#pragma region includes
#include "GameScene.h"

#include "../Game.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"

#include "../../our_scripts/components/rendering/Image.h"
#include "../../our_scripts/components/movement/Transform.h"
#include "../../our_scripts/components/movement/Follow.h"
#include "../../our_scripts/components/movement/MovementController.h"
#include "../../our_scripts/components/LifetimeTimer.h"
#include "../../our_scripts/states/Conditions.h"
#include "../../our_scripts/states/WalkingState.h"
#include "../../our_scripts/states/AttackingState.h"
#include "../../our_scripts/states/WaitingState.h"
#include "../../our_scripts/states/RotatingState.h"
#include "../../our_scripts/states/DashingState.h"
#include "../../our_scripts/states/AnimationState.h"

#include "../../our_scripts/components/weapons/enemies/WeaponSuperMichiMafioso.h"

#include "../../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../../our_scripts/components/cards/Mana.h"
#include "../../our_scripts/components/cards/Deck.hpp"
#include "../../our_scripts/components/rendering/dyn_image.hpp"
#include "../../our_scripts/components/rendering/dyn_image_with_frames.hpp"
#include "../../our_scripts/components/rendering/FlipXController.h"
#include "../../our_scripts/components/AnimationComponent.h"
#include "../../our_scripts/components/rendering/camera_component.hpp"

#include "../../our_scripts/components/Health.h"
#include "../../our_scripts/components/weapons/BulletData.h"

#include "../../utils/Collisions.h"

#include "../../our_scripts/components/WaveManager.h"
#include "../../our_scripts/components/Fog.h"
#include "../../our_scripts/components/weapons/enemies/WeaponMichiMafioso.h"
#include "../../our_scripts/components/weapons/enemies/WeaponPlimPlim.h"
#include "../../our_scripts/components/weapons/enemies/WeaponSarnoRata.h"
#include "../../our_scripts/components/weapons/enemies/WeaponBoom.h"
#include "../../our_scripts/components/weapons/enemies/WeaponCatKuza.h"
#include "../../our_scripts/components/weapons/enemies/WeaponRataBasurera.h"
#include "../../our_scripts/components/weapons/enemies/WeaponReyBasurero.h"
#include "../../our_scripts/components/Health.h"
#include "../../our_scripts/components/bullet_collision_component.hpp"
#include "../../our_scripts/components/fog_collision_component.hpp"

#include "../../our_scripts/components/StateMachine.h"
#include "../../our_scripts/components/rendering/dyn_image.hpp"
#include "../../our_scripts/components/rendering/transformless_dyn_image.h"
#include "../../our_scripts/components/rendering/render_ordering.hpp"
#include "../../our_scripts/components/rendering/rect_component.hpp"
#include "../../our_scripts/components/ui/PlayerHUD.h"
#include "../../our_scripts/components/ui/HUD.h"
#include "../../our_scripts/components/collision_triggerers.hpp"
#include "../../our_scripts/components/id_component.h"
#include "../../our_scripts/components/collision_registration_by_id.h"
#include "../../our_scripts/components/GamePadPlayerCtrl.hpp"
#include "../../our_scripts/components/rigidbody_component.hpp"

#include "../../our_scripts/card_system/PlayableCards.hpp"
#include "../../our_scripts/card_system/CardUpgrade.hpp"

#include "../../our_scripts/components/MythicComponent.h"
#include "RewardScene.h"
#ifdef GENERATE_LOG
#include "../../our_scripts/log_writer_to_csv.hpp"
#endif

#include <iostream>
#include <string>

#pragma endregion

GameScene::GameScene() : Scene(ecs::scene::GAMESCENE)
{
	event_system::event_manager::Instance()->suscribe_to_event(event_system::change_deccel, this, &event_system::event_receiver::event_callback0);
	event_system::event_manager::Instance()->suscribe_to_event(event_system::player_dead, this, &event_system::event_receiver::event_callback1);
}
GameScene::~GameScene()
{
	event_system::event_manager::Instance()->unsuscribe_to_event(event_system::change_deccel, this, &event_system::event_receiver::event_callback0);
	event_system::event_manager::Instance()->unsuscribe_to_event(event_system::player_dead, this, &event_system::event_receiver::event_callback1);
}

float GameScene::deccel_spawned_creatures_multi = 1;

struct game_scene_map_walls
{
	std::array<ecs::entity_t, 4> wall_entities;
	std::array<collisionable *, 4> wall_collisionables;

	constexpr static size_t size()
	{
		return 4;
	}
};
static game_scene_map_walls game_scene_create_map_walls(ecs::Manager &manager, const ecs::sceneId_t scene_id, const rect_f32 scene_limits)
{
	const std::array<ecs::entity_t, game_scene_map_walls::size()> wall_entities{
		manager.addEntity(scene_id),
		manager.addEntity(scene_id),
		manager.addEntity(scene_id),
		manager.addEntity(scene_id)};

	constexpr static const float wall_thickness = 1.0f;
	const std::array<position2_f32, game_scene_map_walls::size()> wall_positions{
		position2_f32{scene_limits.position.x - scene_limits.size.x * 0.5f - 0.001f /*- wall_thickness * 0.25f-*/, scene_limits.position.y},
		position2_f32{scene_limits.position.x + scene_limits.size.x * 0.5f + 0.001f /*+ wall_thickness * 0.25f-*/, scene_limits.position.y},
		position2_f32{scene_limits.position.x, scene_limits.position.y - scene_limits.size.y * 0.5f - 0.001f /*- wall_thickness * 0.25f*/},
		position2_f32{scene_limits.position.x, scene_limits.position.y + scene_limits.size.y * 0.5f + 0.001f /*+ wall_thickness * 0.25f*/}};

	constexpr static const auto vec_from_position = [](const position2_f32 position) -> Vector2D
	{
		return Vector2D{position.x, position.y};
	};
	const std::array<Transform *, game_scene_map_walls::size()> wall_transforms{
		new Transform{vec_from_position(wall_positions[0]), Vector2D{0.0f, 0.0f}, 0.0f, 0.0f},
		new Transform{vec_from_position(wall_positions[1]), Vector2D{0.0f, 0.0f}, 0.0f, 0.0f},
		new Transform{vec_from_position(wall_positions[2]), Vector2D{0.0f, 0.0f}, 0.0f, 0.0f},
		new Transform{vec_from_position(wall_positions[3]), Vector2D{0.0f, 0.0f}, 0.0f, 0.0f}};

	const std::array<rect_component *, game_scene_map_walls::size()> wall_rects{
		new rect_component{0.0f, 0.0f, wall_thickness, scene_limits.size.y - wall_thickness},
		new rect_component{0.0f, 0.0f, wall_thickness, scene_limits.size.y - wall_thickness},
		new rect_component{0.0f, 0.0f, scene_limits.size.x - wall_thickness, wall_thickness},
		new rect_component{0.0f, 0.0f, scene_limits.size.x - wall_thickness, wall_thickness}};

	const std::array<rigidbody_component *, game_scene_map_walls::size()> wall_rigidbodies{
		new rigidbody_component{rect_f32_full_subrect, inverse_mass_f32{0.00000000f}, 1.0f},
		new rigidbody_component{rect_f32_full_subrect, inverse_mass_f32{0.00000000f}, 1.0f},
		new rigidbody_component{rect_f32_full_subrect, inverse_mass_f32{0.00000000f}, 1.0f},
		new rigidbody_component{rect_f32_full_subrect, inverse_mass_f32{0.00000000f}, 1.0f}};
	const std::array<collisionable *, game_scene_map_walls::size()> wall_collisionables{
		new collisionable{*wall_transforms[0], *wall_rigidbodies[0], *wall_rects[0], collisionable_option_none},
		new collisionable{*wall_transforms[1], *wall_rigidbodies[1], *wall_rects[1], collisionable_option_none},
		new collisionable{*wall_transforms[2], *wall_rigidbodies[2], *wall_rects[2], collisionable_option_none},
		new collisionable{*wall_transforms[3], *wall_rigidbodies[3], *wall_rects[3], collisionable_option_none}};

	for (size_t i = 0; i < wall_entities.size(); ++i)
	{
		manager.addComponent<Transform>(wall_entities[i], *wall_transforms[i]);
		manager.addComponent<rect_component>(wall_entities[i], *wall_rects[i]);
		manager.addComponent<rigidbody_component>(wall_entities[i], *wall_rigidbodies[i]);
		manager.addComponent<collisionable>(wall_entities[i], *wall_collisionables[i]);
		manager.addComponent<render_ordering>(wall_entities[i], 0);
	}

	return game_scene_map_walls{
		wall_entities,
		wall_collisionables};
}

ecs::entity_t GameScene::create_environment(ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto environment = manager.addEntity(scene);
	auto &&tr = *manager.addComponent<Transform>(environment, Vector2D(-16.0, 9.0), Vector2D(0.0, 0.0), 0.0f, 0.05f);
	auto &&rect = *manager.addComponent<rect_component>(environment, -0.5f, 0.5f, 35.0f, 20.0f);
	(void)tr;
	manager.addComponent<offset_dyn_image>(environment, rect_f32{{0.0, 0.0}, {1.0, 1.0}}, position2_f32{0.0f, 0.0f}, rect, manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam, sdlutils().images().at("floor"), tr);
	manager.addComponent<render_ordering>(environment, 0);
	game_scene_create_map_walls(manager, scene, GameScene::default_scene_bounds);
	return environment;
}

void GameScene::initScene()
{
	id_component::reset();
	const rendering::camera_creation_descriptor_flags flags =
		rendering::camera_creation_descriptor_options::camera_creation_descriptor_options_set_handler | rendering::camera_creation_descriptor_options::camera_creation_descriptor_options_clamp;
	ecs::entity_t camera = rendering::create_camera(ecs::scene::GAMESCENE, flags, nullptr);
	ecs::entity_t player = create_player();
	auto &&manager = *Game::Instance()->get_mngr();
	manager.setHandler(ecs::hdlr::PLAYER, player);

	manager.addComponent<MythicComponent>(player);

	manager.refresh();
	create_environment();
	// spawn_catkuza(Vector2D{5.0f, 0.0f});
	// spawn_sarno_rata(Vector2D{5.0f, 0.0f});
	spawn_fog();
	spawn_wave_manager();
	auto hud = create_hud();
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::HUD_ENTITY, hud);
}

void GameScene::enterScene()
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto player = manager.getHandler(ecs::hdlr::PLAYER);

	auto camera = manager.getHandler(ecs::hdlr::CAMERA);
	Game::Instance()->get_mngr()->change_ent_scene(camera, ecs::scene::GAMESCENE);

	manager.addComponent<camera_follow>(camera, camera_follow_descriptor{.previous_position = manager.getComponent<camera_component>(camera)->cam.camera.position, .lookahead_time = 1.0f, .semi_reach_time = 2.5f}, *manager.getComponent<camera_component>(camera), *manager.getComponent<Transform>(player));

	manager.refresh();

	manager.addComponent<id_component>(player);

	auto d = manager.getComponent<Deck>(player);
	d->reload();

	manager.getComponent<fog_collision_component>(manager.getHandler(ecs::hdlr::FOGGROUP))->reset();

	manager.addComponent<KeyboardPlayerCtrl>(player);
	manager.addComponent<GamePadPlayerCtrl>(player);
	manager.addComponent<PlayerHUD>(player);
	auto wm = manager.getComponent<WaveManager>(manager.getHandler(ecs::hdlr::WAVE));
	wm->start_new_wave();
	// get the current event
	auto e = wm->get_current_event();
	RewardScene::will_have_mythic(e != NONE);
	manager.getComponent<HUD>(manager.getHandler(ecs::hdlr::HUD_ENTITY))->start_new_wave();

	//spawn_super_michi_mafioso(Vector2D{5.0f, 0.0f});
#ifdef GENERATE_LOG
	log_writer_to_csv::Instance()->add_new_log();
	log_writer_to_csv::Instance()->add_new_log("ENTERED GAME SCENE");
#endif
}

void GameScene::exitScene()
{
	auto &&manager = *Game::Instance()->get_mngr();
	manager.getComponent<WaveManager>(manager.getHandler(ecs::hdlr::WAVE))->reset_wave_time();
#ifdef GENERATE_LOG
	log_writer_to_csv::Instance()->add_new_log("EXIT GAME SCENE");
	log_writer_to_csv::Instance()->add_new_log();
#endif
}

// metodos de create/spawn

#pragma region Player
ecs::entity_t GameScene::create_player(ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&camera = manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam;

	auto &&player_transform = *new Transform({0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f, 2.0f);
	auto &&player_rect = *new rect_component{0, 0, 1.125f, 1.5f};
	auto &&player_rigidbody = *new rigidbody_component{rect_f32{{0.15f, -0.125}, {0.5f, 0.75f}}, mass_f32{7.0f}, 1.0f};
	auto &&player_collisionable = *new collisionable{player_transform, player_rigidbody, player_rect, collisionable_option_none};
	ecs::entity_t player = create_entity(
		ecs::grp::PLAYER,
		scene,
		&player_transform,
		&player_rect,
		new dyn_image_with_frames(
			rect_f32{{0, 0}, {0.2, 1}},
			player_rect,
			camera,
			sdlutils().images().at("piu"),
			player_transform),
		new render_ordering{1},
		new Health(100, true),
		new ManaComponent(),
		&player_rigidbody,
		&player_collisionable,
		new MovementController(0.1f, 5.0f, 20.0f * deccel_spawned_creatures_multi),
		new player_collision_triggerer(),
		new id_component());

	// si tiene mas de una animacion
	auto *anim = manager.addComponent<AnimationComponent>(player);
	anim->add_animation("andar", 1, 5, 100);
	anim->add_animation("idle", 0, 0, 100);

	return player;
}

void GameScene::reset_player()
{
	auto &&mngr = *Game::Instance()->get_mngr();
	auto player = mngr.getHandler(ecs::hdlr::PLAYER);

	mngr.removeComponent<Weapon>(player);
	mngr.removeComponent<Deck>(player);
	mngr.removeComponent<KeyboardPlayerCtrl>(player);
	mngr.removeComponent<GamePadPlayerCtrl>(player);
	mngr.removeComponent<PlayerHUD>(player);

	mngr.getComponent<MythicComponent>(player)->reset();
	mngr.getComponent<dyn_image_with_frames>(player)->isDamaged = false;
	auto tr = mngr.getComponent<Transform>(player);
	tr->setPos({0.0f, 0.0f});
	tr->setDir({0.0f, 0.0f});

	mngr.getComponent<AnimationComponent>(player)->play_animation("idle");
	mngr.getComponent<Health>(player)->resetCurrentHeatlh();
	mngr.addComponent<ManaComponent>(player);
	mngr.addComponent<MovementController>(player, 0.1f, 5.0f, 20.0f * deccel_spawned_creatures_multi);
}

#pragma endregion

#pragma region Enemy
struct EnemySpawnConfig
{
	Transform *tr;
	std::string spriteKey;
	Weapon *weapon;
	float health;
	float width;
	float height;
	MovementController *mc;
};

ecs::entity_t GameScene::create_enemy(GameStructs::EnemyProperties &&ec, ecs::sceneId_t scene, Weapon *weapon)
{
	auto &&manager = *Game::Instance()->get_mngr();

	float randSize = float(sdlutils().rand().nextInt(6, 10)) / 10.0f;
	auto &&rect = *new rect_component{0, 0, ec.width * randSize, ec.height * randSize};
	auto &&rigidbody = *new rigidbody_component{rect_f32{{0.0f, -0.15f}, {0.5f, 0.6f}}, mass_f32{3.0f}, 0.05f};
	auto &&tr = *new Transform(ec.start_pos, ec.dir, ec.r, ec.s * randSize);
	auto &&fll = *new Follow(ec.follow);
	auto &&col = *new collisionable{tr, rigidbody, rect, collisionable_option_none};
	auto &&mc = *new MovementController(ec.max_speed, ec.acceleration, ec.decceleration * deccel_spawned_creatures_multi);
	auto &&state = *new StateMachine();

	auto e = create_entity(
		ecs::grp::ENEMY,
		scene,
		&tr,
		&rect,
		new dyn_image(
			rect_f32{{0, 0}, {1, 1}},
			rect,
			manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at(ec.sprite_key),
			tr),
		new Health(ec.health),
		new FlipXController(),
		new enemy_collision_triggerer(),
		new id_component(),
		weapon,
		&rigidbody,
		&col,
		&mc,
		&state,
		&fll);

	return e;
}
#pragma endregion

#pragma region Super Michi Mafioso
void GameScene::spawn_super_michi_mafioso(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponSuperMichiMafioso();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"super_michi_mafioso", // sprite_key
			posVec,				   // start_pos
			GameStructs::DEFAULT,  // enemy_type
			20,					   // health
			1.75f,				   // width
			2.25f,				   // height
			GameStructs::CLOSEST,  // target_strategy
			{0.0f, 0.0f},		   // velocity
			0.0f,				   // rotation
			2.0f				   // scale
		},
		scene,
		static_cast<Weapon *>(&weapon));

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);

	Follow *fll = manager.getComponent<Follow>(e);
	auto _p_tr = fll->get_act_follow();

	auto state_cm = state->getConditionManager();

	state_cm->set_cooldown("area_attack_duration", 400);
	state_cm->set_cooldown("large_area_attack_duration", 10000);
	state_cm->set_cooldown("shot_attack", 2000);
	state_cm->set_cooldown("spawn_michi", 8000);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto waitingState = std::make_shared<WaitingState>();

	auto areaAttackState = std::make_shared<AttackingState>(
		tr, fll, &weapon, false,
		[&weapon, fll]()
		{
			weapon.set_player_pos(fll->get_act_follow()->getPos());
			weapon.attack1();
		});

	auto shotAttackState = std::make_shared<AttackingState>(
		tr, fll, &weapon, false,
		[&weapon, tr]()
		{
			Vector2D shootPos = tr->getPos(); // Posición del enemigo
			weapon.attack2(shootPos);
		});

	auto largeAreaAttackState = std::make_shared<AttackingState>(
		tr, fll, &weapon, false,
		[&weapon, tr]()
		{
			Vector2D shootPos = tr->getPos(); // Posición del enemigo
			weapon.attack2(shootPos);
		});

	auto spawnMichiState = std::make_shared<AttackingState>(
		tr, fll, &weapon, false,
		[&weapon]() { weapon.generate_michi_mafioso(); });

	// poner los estado a la state
	state->add_state("Walking", walkingState);
	state->add_state("AreaAttack", areaAttackState);
	state->add_state("ShotAttack", shotAttackState);
	state->add_state("LargeAreaAttack", largeAreaAttackState);
	state->add_state("SpawnMichi", spawnMichiState);
	state->add_state("Waiting", waitingState);

	float dist_to_attack = 4.0f;

	// Condiciones de cada estado
	// Patron1: cuando se acerca al player empieza el p1
	state->add_transition("Walking", "AreaAttack", [state_cm, _p_tr, tr, dist_to_attack]()
						  { return state_cm->can_use("area_attack_duration", sdlutils().virtualTimer().currTime()) && state_cm->is_player_near(_p_tr, tr, dist_to_attack); });

	state->add_transition("AreaAttack", "Waiting", [state_cm, dist_to_attack]()
						  {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();
		bool trans = state_cm->can_use("area_attack_duration", currentTime);
		if (trans) { state_cm->reset_cooldown("area_attack_duration", currentTime); };
		return trans; });

	state->add_transition("Waiting", "AreaAttack", [state_cm, _p_tr, tr, dist_to_attack]()
						  { return state_cm->can_use("area_attack_duration", sdlutils().virtualTimer().currTime()) && state_cm->is_player_near(_p_tr, tr, dist_to_attack); });

	// patron2: cuando ataque 3 veces p1, pasa a 2 si el player no se aleja mucho
	state->add_transition("Waiting", "ShotAttack", [state_cm, _p_tr, tr, dist_to_attack]()
						  {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();

		bool trans = state_cm->can_use("shot_attack", currentTime) && state_cm->is_player_near(_p_tr, tr, dist_to_attack);
		if (trans) { state_cm->reset_cooldown("shot_attack", currentTime); };
		return trans; });

	state->add_transition("ShotAttack", "Waiting", []()  { return true; });

	// spawn
	state->add_transition("Waiting", "SpawnMichi", [state_cm]()
						  {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();
		bool trans = state_cm->can_use("spawn_michi", currentTime);
		if (trans) {
			state_cm->reset_cooldown("spawn_michi", currentTime);
		}
		return trans; });
	state->add_transition("SpawnMichi", "Waiting", []() { return true; });

	// patron 3
	state->add_transition("Waiting", "LargeAreaAttack", [state_cm, _p_tr, tr]()
						  {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();
		bool trans = state_cm->can_use("large_area_attack_duration", currentTime) && state_cm->is_player_near(_p_tr, tr, 2.0f);;
		if (trans) {
			state_cm->reset_cooldown("large_area_attack_duration", currentTime);
		}
		return trans; });

	state->add_transition("LargeAreaAttack", "Waiting", []() { return true; });

	// A walking si el player se aleja
	state->add_transition("Waiting", "Walking", [state_cm, _p_tr, tr, dist_to_attack]()
		{ return !state_cm->is_player_near(_p_tr, tr, dist_to_attack*1.2); });

	state->add_transition("SpawnMichi", "Walking", [state_cm, _p_tr, tr, dist_to_attack]()
		{ return !state_cm->is_player_near(_p_tr, tr, dist_to_attack * 1.2); });

	state->add_transition("LargeAreaAttack", "Walking", [state_cm, _p_tr, tr, dist_to_attack]()
		{ return !state_cm->is_player_near(_p_tr, tr, dist_to_attack * 1.2); });

	state->add_transition("ShotAttack", "Walking", [state_cm, _p_tr, tr, dist_to_attack]()
		{ return !state_cm->is_player_near(_p_tr, tr, dist_to_attack * 1.2); });

	state->add_transition("AreaAttack", "Walking", [state_cm, _p_tr, tr, dist_to_attack]()
		{ return !state_cm->is_player_near(_p_tr, tr, dist_to_attack * 1.2); });

	// Estado inicial
	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Catkuza
void GameScene::spawn_catkuza(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponCatKuza();

	auto e = create_enemy(GameStructs::EnemyProperties{"catkuza", posVec, GameStructs::DEFAULT, 5, 2.0f, 2.25f, GameStructs::CLOSEST, {0.0f, 0.0f}, 0.0f, 2.0f}, scene, static_cast<Weapon *>(&weapon));

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	// Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();
	auto state_cm = state->getConditionManager();

	state_cm->set_cooldown("wind_attack_duration", 1000);
	state_cm->set_cooldown("charging_duration", 500);
	state_cm->set_cooldown("dash_attack_duration", 1000);
	state_cm->set_cooldown("explosion_attack_duration", 800);
	state_cm->set_cooldown("delayed_slash_duration", 1200);

	state_cm->add_pattern("PATTERN_1", 1);
	state_cm->add_pattern("PATTERN_2", 1);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto dashState = std::make_shared<DashingState>(tr, mc, fll);
	auto chargingState = std::make_shared<WaitingState>();

	auto windAttackState = std::make_shared<AttackingState>(
		tr, fll, &weapon, false,
		[&weapon, tr]()
		{
			Vector2D shootPos = tr->getPos(); // Posición del enemigo
			weapon.wind_attack(shootPos);
		});

	auto areaAttackState = std::make_shared<AttackingState>(
		tr, fll, &weapon, false,
		[&weapon, tr]()
		{
			Vector2D shootPos = tr->getPos(); // Posición del enemigo
			weapon.area_attack(shootPos);
		});

	auto dashAttackState = std::make_shared<AttackingState>(
		tr, fll, &weapon, false,
		[&weapon, tr, fll, mc]()
		{
			Vector2D shootPos = tr->getPos();
			Vector2D shootDir = (fll->get_act_follow()->getPos() - shootPos).normalize();

			Vector2D dash_target = fll->get_act_follow()->getPos() + shootDir * 1.8f;
			weapon.dash_attack(shootPos, dash_target);
		});

	auto waitingState = std::make_shared<WaitingState>();

	// poner los estado a la state
	state->add_state("Walking", walkingState);
	state->add_state("Charging", chargingState);
	state->add_state("Dash", dashState);
	state->add_state("Dash2", dashState);
	state->add_state("Dash3", dashState);
	state->add_state("WindAttack", windAttackState);
	state->add_state("WindAttack2", windAttackState);
	state->add_state("DashAttack", dashAttackState);
	state->add_state("AreaAttack", areaAttackState);
	state->add_state("Waiting", waitingState);

	// Transiciones Patrón 1
	state->add_transition("Walking", "Charging",
						  [state_cm, fll, tr, &weapon]()
						  {
							  bool trans = state_cm->is_player_near(fll->get_act_follow(), tr, 5.0f) && state_cm->get_current_pattern() == "PATTERN_1";
							  if (trans)
							  {
								  state_cm->reset_cooldown("charging_duration", sdlutils().currRealTime());
								  weapon.set_player_pos(fll->get_act_follow()->getPos());
							  }
							  return trans;
						  });

	state->add_transition("Charging", "WindAttack",
						  [state_cm, &weapon, fll]()
						  {
							  bool trans = state_cm->can_use("charging_duration", sdlutils().currRealTime());
							  if (trans)
							  {
								  state_cm->reset_cooldown("wind_attack_duration", sdlutils().currRealTime());
								  weapon.set_player_pos(fll->get_act_follow()->getPos());
							  }
							  return trans;
						  });

	state->add_transition("WindAttack", "Dash",
						  [state_cm, &weapon, fll]()
						  {
							  bool trans = state_cm->can_use("wind_attack_duration", sdlutils().currRealTime());
							  if (trans)
							  {

								  state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());

								  weapon.set_player_pos(fll->get_act_follow()->getPos());
							  }
							  return trans;
						  });

	state->add_transition("Dash", "WindAttack2",
						  [state_cm]()
						  {
							  bool trans = state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
							  if (trans)
							  {
								  state_cm->reset_cooldown("wind_attack_duration", sdlutils().currRealTime());
							  }
							  return trans;
						  });

	state->add_transition("WindAttack2", "Walking",
						  [state_cm]()
						  {
							  bool trans = state_cm->can_use("wind_attack_duration", sdlutils().currRealTime());
							  if (trans)
							  {
								  state_cm->reset_cooldown("wind_attack_duration", sdlutils().currRealTime());
								  state_cm->switch_pattern();
							  }
							  return trans;
						  });

	// Transiciones Patrón 2
	state->add_transition("Walking", "Dash2",
						  [state_cm, fll, &weapon]()
						  {
							  bool trans = state_cm->get_current_pattern() == "PATTERN_2" && state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
							  if (trans)
							  {
								  state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
								  weapon.set_player_pos(fll->get_act_follow()->getPos());
							  }
							  return trans;
						  });

	state->add_transition("Dash2", "AreaAttack",
						  [state_cm]()
						  {
							  bool trans = state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
							  if (trans)
							  {
								  state_cm->reset_cooldown("explosion_attack_duration", sdlutils().currRealTime());
							  }
							  return trans;
						  });

	state->add_transition("AreaAttack", "Dash3",
						  [state_cm, fll, &weapon]()
						  {
							  bool trans = state_cm->can_use("explosion_attack_duration", sdlutils().currRealTime());
							  if (trans)
							  {
								  state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
								  weapon.set_player_pos(fll->get_act_follow()->getPos());
							  }
							  return trans;
						  });

	state->add_transition("Dash3", "DashAttack",
						  [state_cm]()
						  {
							  state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
							  return true;
						  });

	state->add_transition("DashAttack", "Walking",
						  [state_cm]()
						  {
							  bool trans = state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
							  if (trans)
							  {
								  state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
								  state_cm->switch_pattern();
							  }
							  return trans;
						  });

	// Estado inicial
	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Sarno Rata
void GameScene::spawn_sarno_rata(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponSarnoRata();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"sarno_rata",		  // sprite_key
			posVec,				  // start_pos
			GameStructs::DEFAULT, // enemy_type
			7,					  // health
			1.125f,				  // width
			1.5f,				  // height
			GameStructs::CLOSEST, // target_strategy
			{0.0f, 0.0f},		  // velocity
			0.0f,				  // rotation
			1.0f				  // scale
		},
		scene,
		static_cast<Weapon *>(&weapon));

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();
	
	auto state_cm = state->getConditionManager();

	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto attackingState = std::make_shared<AttackingState>(tr, fll, &weapon);

	state->add_state("Walking", walkingState);
	state->add_state("Attacking", attackingState);

	state->add_transition("Walking", "Attacking", [state_cm, fll, tr]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, 1.5f); });

	state->add_transition("Attacking", "Walking", [state_cm, fll, tr]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, 1.3f); });

	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Michi Mafioso
void GameScene::spawn_michi_mafioso(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponMichiMafioso();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"michi_mafioso",	  // sprite_key
			posVec,				  // start_pos
			GameStructs::DEFAULT, // enemy_type
			5,					  // health
			1.0f,				  // width
			1.125f,				  // height
			GameStructs::CLOSEST, // target_strategy
			{0.0f, 0.0f},		  // velocity
			0.0f,				  // rotation
			2.0f				  // scale
		},
		scene,
		static_cast<Weapon *>(&weapon));

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();

	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto backingState = std::make_shared<WalkingState>(tr, mc, fll, true);
	auto attackingState = std::make_shared<AttackingState>(tr, fll, &weapon);

	state->add_state("Walking", walkingState);
	state->add_state("Attacking", attackingState);
	state->add_state("Backing", backingState);

	float dist_to_attack = 3.0f;
	float dist_to_fallback = 2.5f;

	state->add_transition("Walking", "Attacking", [state_cm, fll, tr, dist_to_attack]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_attack); });

	state->add_transition("Attacking", "Walking", [state_cm, fll, tr, dist_to_attack]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_attack); });

	state->add_transition("Walking", "Backing", [state_cm, fll, tr, dist_to_fallback]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_fallback); });

	state->add_transition("Backing", "Walking", [state_cm, fll, tr, dist_to_fallback, dist_to_attack]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_fallback) &&
								   !state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_attack); });

	state->add_transition("Attacking", "Backing", [state_cm, fll, tr, dist_to_fallback]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_fallback); });

	state->add_transition("Backing", "Attacking", [state_cm, fll, tr, dist_to_fallback]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_fallback); });

	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Plim Plim
void GameScene::spawn_plim_plim(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponPlimPlim();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"plim_plim",		  // sprite_key
			posVec,				  // start_pos
			GameStructs::DEFAULT, // enemy_type
			4,					  // health
			1.0f,				  // width
			1.0f,				  // height
			GameStructs::CLOSEST, // target_strategy
			{0.0f, 0.0f},		  // velocity
			0.0f,				  // rotation
			2.0f				  // scale
		},
		scene,
		static_cast<Weapon *>(&weapon));

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();
	
	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto attackingState = std::make_shared<AttackingState>(tr, fll, &weapon);

	state->add_state("Walking", walkingState);
	state->add_state("Attacking", attackingState);

	state->add_transition("Walking", "Attacking", [state_cm, fll, tr]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, 4.0f); });

	state->add_transition("Attacking", "Walking", [state_cm, fll, tr]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, 6.0f); });

	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Boom
void GameScene::spawn_boom(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponBoom();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"boom",				  // sprite_key
			posVec,				  // start_pos
			GameStructs::DEFAULT, // enemy_type
			14,					  // health
			1.8f,				  // width
			1.8f,				  // height
			GameStructs::CLOSEST, // target_strategy
			{0.0f, 0.0f},		  // velocity
			0.0f,				  // rotation
			2.0f				  // scale
		},
		scene,
		static_cast<Weapon *>(&weapon));

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();
	Health *health = manager.getComponent<Health>(e);

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();
	
	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto attackingState = std::make_shared<AttackingState>(tr, fll, &weapon, false,
														   [health]()
														   { health->takeDamage(health->getMaxHealth()); });

	state->add_state("Walking", walkingState);
	state->add_state("Attacking", attackingState);

	state->add_transition("Walking", "Attacking", [state_cm, fll, tr]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, 1.0f); });

	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Ratatouille
void GameScene::spawn_ratatouille(Vector2D posVec, ecs::sceneId_t scene)
{
	float randSpeed = float(sdlutils().rand().nextInt(10, 20) / 10.0f);
	int damage = 4;
	auto &&manager = *Game::Instance()->get_mngr();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"ratatouille",		  // sprite_key
			posVec,				  // start_pos
			GameStructs::DEFAULT, // enemy_type (especial, no usa weapon)
			2,					  // health
			0.8f,				  // width
			0.8f,				  // height
			GameStructs::CLOSEST, // target_strategy
			{0.0f, 0.0f},		  // velocity
			0.0f,				  // rotation
			2.0f				  // scale
		},
		scene,
		nullptr // Sin arma
	);

	// Componentes adicionales específicos para Ratatouille
	manager.addComponent<ratatouille_collision_component>(e, damage, 2);

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();
	
	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto rotatingState = std::make_shared<RotatingState>(tr, fll->get_act_follow(), mc);

	state->add_state("Walking", walkingState);
	state->add_state("Rotating", rotatingState);

	float dist_to_rotate = 3.5f;

	// Condiciones de cada estado
	// De: Walking a: Rotating, Condición: Jugador cerca
	state->add_transition("Walking", "Rotating", [state_cm, fll, tr, dist_to_rotate]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_rotate); });

	// De: Rotating a: Walking, Condición: Jugador lejos
	state->add_transition("Rotating", "Walking", [state_cm, fll, tr, dist_to_rotate]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, dist_to_rotate * 1.8f); });

	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Rata Basurera
void GameScene::spawn_rata_basurera(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponRataBasurera();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"rata_basurera",	  // sprite_key
			posVec,				  // start_pos
			GameStructs::DEFAULT, // enemy_type
			2,					  // health
			1.8f,				  // width
			1.8f,				  // height
			GameStructs::CLOSEST, // target_strategy
			{0.0f, 0.0f},		  // velocity
			0.0f,				  // rotation
			2.0f				  // scale
		},
		scene,
		static_cast<Weapon *>(&weapon));

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	// Configuración especial para Rata Basurera
	weapon.sendHealthComponent(manager.getComponent<Health>(e));

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();

	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto attackingState = std::make_shared<AttackingState>(tr, fll, &weapon);

	state->add_state("Walking", walkingState);
	state->add_state("Attacking", attackingState);

	// Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador a distancia correcta
	state->add_transition("Walking", "Attacking", [state_cm, fll, tr]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, 50.0f); });

	// De: Attacking a: Walking, Condición: Jugador se aleja demasiado
	state->add_transition("Attacking", "Walking", [state_cm, fll, tr]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, 55.0f); });

	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Rey Basurero
void GameScene::spawn_rey_basurero(Vector2D posVec, ecs::sceneId_t scene)
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponReyBasurero();

	auto e = create_enemy(
		GameStructs::EnemyProperties{
			"rey_basurero",		  // sprite_key
			posVec,				  // start_pos
			GameStructs::DEFAULT, // enemy_type
			2,					  // health
			1.8f,				  // width
			1.8f,				  // height
			GameStructs::CLOSEST, // target_strategy
			{0.0f, 0.0f},		  // velocity
			0.0f,				  // rotation
			1.0f				  // scale
		},
		scene,
		static_cast<Weapon *>(&weapon));

	Transform *tr = manager.getComponent<Transform>(e);
	MovementController *mc = manager.getComponent<MovementController>(e);
	StateMachine *state = manager.getComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	Follow *fll = manager.getComponent<Follow>(e);
	fll->act_follow();
	
	auto walkingState = std::make_shared<WalkingState>(tr, mc, fll);
	auto attackingState = std::make_shared<AttackingState>(tr, fll, &weapon);

	state->add_state("Walking", walkingState);
	state->add_state("Attacking", attackingState);

	// Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador a distancia correcta
	state->add_transition("Walking", "Attacking", [state_cm, fll, tr]()
						  { return state_cm->is_player_near(fll->get_act_follow(), tr, 7.0f); });

	// De: Attacking a: Walking, Condición: Jugador lejose aleja demasiado
	state->add_transition("Attacking", "Walking", [state_cm, fll, tr]()
						  { return !state_cm->is_player_near(fll->get_act_follow(), tr, 10.0f); });

	state->set_initial_state("Walking");
}
#pragma endregion

#pragma region Hud
ecs::entity_t GameScene::create_hud(ecs::sceneId_t scene)
{
	auto ent = create_entity(
		ecs::grp::DEFAULT,
		scene,
		new HUD());
	return ent;
}
#pragma endregion

#pragma region Waves
void GameScene::spawn_wave_manager()
{
	auto ent = create_entity(
		ecs::grp::DEFAULT,
		ecs::scene::GAMESCENE,
		new WaveManager());
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::WAVE, ent);
}
void GameScene::spawn_fog()
{
	auto &&transform = *new Transform({0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f, 1.0f);
	auto &&rect = *new rect_component{0.0f, 0.0f, 35.0f, 20.0f};
	dyn_image *this_fog_image = new dyn_image(
		rect_f32{{0, 0}, {1, 1}},
		rect,
		Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam,
		sdlutils().images().at("fog"),
		transform);

	auto &&fog_rigidbody = *new rigidbody_component{rect_f32{{0.0f, 0.0f}, {1.0f, 1.0f}}, mass_f32{1.0f}, 0.0f};
	auto &&fog_collisionable = *new collisionable{transform, fog_rigidbody, rect, collisionable_option_trigger};

	Fog *this_fog = new Fog();
	// La entidad tiene un grupo, una escena, un Transform, rect_component, un Fog y un dyn_image
	auto ent = create_entity(
		ecs::grp::DEFAULT,
		ecs::scene::GAMESCENE,
		&transform,
		&rect,
		this_fog,
		this_fog_image,
		&fog_rigidbody,
		&fog_collisionable,
		new fog_collision_component()

	);
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::FOGGROUP, ent);
}
#pragma endregion

#pragma region Proyectile
void GameScene::generate_proyectile(const GameStructs::BulletProperties &bp, ecs::grpId_t gid, ecs::sceneId_t scene)
{
	auto manager = Game::Instance()->get_mngr();
	(void)gid;

	auto &&transform = *new Transform(bp.init_pos, bp.dir, (atan2(-bp.dir.getY(), bp.dir.getX()) + M_PI / 2) * 180.0f / M_PI, bp.speed);
	auto &&rect = *new rect_component{0, 0, bp.width, bp.height};
	auto &&player_rigidbody = *new rigidbody_component{rect_f32{{0.15f, -0.125}, {0.5f, 0.75f}}, mass_f32{7.0f}, 1.0f};
	auto &&player_collisionable = *new collisionable{transform, player_rigidbody, rect, collisionable_option_trigger};

	auto e = create_entity(
		gid,
		scene,
		&transform,
		&rect,
		new dyn_image(
			rect_f32{{0, 0}, {1, 1}},
			rect,
			manager->getComponent<camera_component>(manager->getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at(bp.sprite_key),
			transform),
		new LifetimeTimer(bp.life_time),
		new BulletData(bp.damage, bp.weapon_type),
		&player_rigidbody,
		&player_collisionable,
		new bullet_collision_component(bp));
	if (bp.collision_filter == GameStructs::collide_with::enemy || bp.collision_filter == GameStructs::collide_with::all)
		manager->addComponent<collision_registration_by_id>(e);
}
void GameScene::create_proyectile(const GameStructs::BulletProperties &bp, ecs::grpId_t gid)
{
	generate_proyectile(bp, gid);
}
#pragma endregion

void GameScene::event_callback0(const event_system::event_receiver::Msg &m)
{
	deccel_spawned_creatures_multi *= m.float_value;
}
void GameScene::event_callback1(const event_system::event_receiver::Msg &m)
{
	auto &&mngr = *Game::Instance()->get_mngr();
	reset_player();
	deccel_spawned_creatures_multi = 1;
	mngr.getComponent<WaveManager>(mngr.getHandler(ecs::hdlr::WAVE))->reset_wave_manager();

	Game::Instance()->change_Scene(Game::GAMEOVER);
}