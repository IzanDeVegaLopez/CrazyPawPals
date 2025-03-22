#include "GameScene.h"

#include "../Game.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"

#include "../../our_scripts/components/rendering/Image.h"
#include "../../our_scripts/components/movement/Transform.h"
#include "../../our_scripts/components/movement/MovementController.h"
#include "../../our_scripts/components/LifetimeTimer.h"
#include "../../our_scripts/states/Conditions.h"
#include "../../our_scripts/states/WalkingState.h"
#include "../../our_scripts/states/AttackingState.h"
#include "../../our_scripts/states/WaitingState.h"
#include "../../our_scripts/states/RotatingState.h"
#include "../../our_scripts/states/DashingState.h"

#include "../../our_scripts/states/SuperMichiMafiosoAttack.h"
#include "../../our_scripts/components/weapons/enemies/WeaponSuperMichiMafioso.h"

#include "../../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../../our_scripts/components/cards/Mana.h"
#include "../../our_scripts/components/cards/Deck.hpp"
#include "../../our_scripts/components/rendering/dyn_image.hpp"
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
#include "../../our_scripts/components/Health.h"

#include "../../our_scripts/components/StateMachine.h"
#include "../../our_scripts/components/rendering/dyn_image.hpp"
#include "../../our_scripts/components/rendering/transformless_dyn_image.h"
#include "../../our_scripts/components/rendering/render_ordering.hpp"
#include "../../our_scripts/components/rendering/rect_component.hpp"
#include "../../our_scripts/components/movement/StopOnBorder.h"
#include "../../our_scripts/components/ui/PlayerHUD.h"
#include "../../our_scripts/components/ui/HUD.h"

#include "../../our_scripts/card_system/PlayableCards.hpp"
#include "../../our_scripts/card_system/CardUpgrade.hpp"

#include "../../our_scripts/components/rigidbody_component.hpp"


#include <iostream>
#include <string>


GameScene::GameScene() : Scene(ecs::scene::GAMESCENE){
	event_system::event_manager::Instance()->suscribe_to_event(event_system::change_deccel, this, &event_system::event_receiver::event_callback0);
}

static ecs::entity_t create_environment() {
	auto&& manager = *Game::Instance()->get_mngr();
	auto environment = manager.addEntity(ecs::scene::GAMESCENE);
	auto &&tr = *manager.addComponent<Transform>(environment, Vector2D(-16.0, 9.0), Vector2D(0.0, 0.0), 0.0f, 0.05f);
	auto &&rect = *manager.addComponent<rect_component>(environment, 0.0f, 0.0f, 32.0f, 18.0f);
	(void)tr;
	manager.addComponent<offset_dyn_image>(environment, rect_f32{
		{0.0, 0.0},
		{1.0, 1.0}
	}, position2_f32{0.0f, 0.0f}, rect, manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam, sdlutils().images().at("floor"), tr);
	manager.addComponent<render_ordering>(environment, 0);
	return environment;
}


void GameScene::initScene() {
	const rendering::camera_creation_descriptor_flags flags =
		rendering::camera_creation_descriptor_options::camera_creation_descriptor_options_set_handler
		| rendering::camera_creation_descriptor_options::camera_creation_descriptor_options_clamp;
	ecs::entity_t camera = rendering::create_camera(ecs::scene::GAMESCENE, flags, nullptr);
	ecs::entity_t player = create_player();

	auto &&manager = *Game::Instance()->get_mngr();
	manager.addComponent<camera_follow>(camera, camera_follow_descriptor{
		.previous_position = manager.getComponent<camera_component>(camera)->cam.camera.position,
		.lookahead_time = 1.0f,
		.semi_reach_time = 2.5f
	}, *manager.getComponent<camera_component>(camera), *manager.getComponent<Transform>(player));

	manager.refresh();
	create_environment();
	spawn_catkuza(Vector2D{5.0f, 0.0f});
	//spawn_super_michi_mafioso(Vector2D{5.0f, 0.0f});
	spawn_fog();
	spawn_wave_manager();
	create_hud();

}

void GameScene::enterScene()
{
	auto* mngr = Game::Instance()->get_mngr();
	auto player = mngr->getHandler(ecs::hdlr::PLAYER);
	auto w = mngr->getComponent<Weapon>(player);

	w->initComponent();
	mngr->addComponent<KeyboardPlayerCtrl>(player);
	auto d = mngr->getComponent<Deck>(player);
	d->initComponent();
	mngr->addComponent<PlayerHUD>(player);

	mngr->getComponent<WaveManager>(mngr->getHandler(ecs::hdlr::WAVE))->start_new_wave();
}

void GameScene::exitScene()
{
}

ecs::entity_t GameScene::create_player()
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&camera = manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam;
	
	auto &&player_transform = *new Transform({ 0.0f, 0.0f }, { 0.0f,0.0f }, 0.0f, 2.0f);
	auto &&player_rect = *new rect_component{0, 0, 1.125f, 1.5f};
	auto &&player_rigidbody = *new rigidbody_component{rect_f32{{0.15f, -0.125}, {0.5f, 0.75f}}, mass_f32{7.0f}, 1.0f};
	auto &&player_collisionable = *new collisionable{player_transform, player_rigidbody, player_rect, collisionable_option_none};
	ecs::entity_t player = create_entity(
		ecs::grp::PLAYER,
		ecs::scene::GAMESCENE,
		&player_transform,
		&player_rect,
		new dyn_image(
			rect_f32_full_subrect,
			player_rect,
			camera,
			sdlutils().images().at("piu"),
			player_transform
		),
		new render_ordering{1},
		new Health(100),
		new ManaComponent(),
		new MovementController(0.1f,5.0f,20.0f*deccel_spawned_creatures_multi),
		//new Deck(c),
		new StopOnBorder(camera, 1.5f, 2.0f),
		&player_rigidbody,
		&player_collisionable
		);
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::PLAYER, player);
	return player;
}

ecs::entity_t GameScene::create_enemy(Transform* tr, const std::string& spriteKey, Weapon* weapon, float health, float width, float height){
	auto&& manager = *Game::Instance()->get_mngr();

	float randSize = float(sdlutils().rand().nextInt(6, 10)) / 10.0f;
	auto&& rect = *new rect_component{ 0, 0, width * randSize, height * randSize };
	auto &&rigidbody = *new rigidbody_component{rect_f32{{0.0f, -0.15f}, {0.5f, 0.6f}}, mass_f32{3.0f}, 0.05f};
	auto &&col = *new collisionable{*tr, rigidbody, rect, collisionable_option_none};
	auto e = create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		tr,
		&rect,
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at(spriteKey),
			*tr
		),
		new Health(health),
		weapon,
		&rigidbody,
		&col
	);
	// BUG: ^^^^ justo ahí arriba se añade el weapon
	// if (weapon != nullptr)manager.addExistingComponent<Weapon>(e, weapon);

	return e;
}

void GameScene::spawn_super_michi_mafioso(Vector2D posVec)
{
	auto&& manager = *Game::Instance()->get_mngr();
	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);
	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr
	
	auto&& weapon = *new WeaponSuperMichiMafioso(_p_tr);
	auto&& tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);

	auto e = create_enemy(&tr, "super_michi_mafioso", static_cast<Weapon*>(&weapon), 2, 2.0f, 2.25f);
	auto&& mc = *manager.addExistingComponent<MovementController>(e, new MovementController(0.01, 3.0f, 15.0f*deccel_spawned_creatures_multi));

	//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	state_cm->set_cooldown("area_attack_duration", 400);
	state_cm->set_cooldown("large_area_attack_duration", 10000);
	state_cm->set_cooldown("shot_attack", 2000);
	state_cm->set_cooldown("spawn_michi", 8000);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc);

	auto areaAttackState = std::make_shared<SuperMichiMafiosoAttack>(
		&tr, _p_tr, &weapon,[&weapon]() {weapon.setAttackPattern(WeaponSuperMichiMafioso::ATTACK1);}
	);

	auto shotAttackState = std::make_shared<SuperMichiMafiosoAttack>(
		&tr, _p_tr, &weapon,[&weapon]() {weapon.setAttackPattern(WeaponSuperMichiMafioso::ATTACK2);}
	);

	auto largeAreaAttackState = std::make_shared<SuperMichiMafiosoAttack>(
		&tr, _p_tr, &weapon,[&weapon]() {weapon.setAttackPattern(WeaponSuperMichiMafioso::ATTACK3);}
	);
	auto spawnMichiState = std::make_shared<SuperMichiMafiosoAttack>(
		&tr, _p_tr, &weapon,[&weapon]() {weapon.setAttackPattern(WeaponSuperMichiMafioso::SPAWN_MICHI_MAFIOSO);}
	);

	auto waitingState = std::make_shared<WaitingState>();

	//poner los estado a la state
	state->add_state("Walking", walkingState);
	state->add_state("AreaAttack", areaAttackState);
	state->add_state("ShotAttack", shotAttackState);
	state->add_state("LargeAreaAttack", largeAreaAttackState);
	state->add_state("SpawnMichi", spawnMichiState);
	state->add_state("Waiting", waitingState);


	float dist_to_attack = 4.0f;

	// Condiciones de cada estado
	// Patron1: cuando se acerca al player empieza el p1
	state->add_transition("Walking", "AreaAttack", [state_cm, _p_tr, &tr, dist_to_attack]() {
		return state_cm->can_use("area_attack_duration", sdlutils().virtualTimer().currTime()) && state_cm->is_player_near(_p_tr, &tr, dist_to_attack);
	});

	state->add_transition("AreaAttack", "Waiting", [state_cm, _p_tr, &tr, dist_to_attack]() {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();
		bool trans = state_cm->can_use("area_attack_duration", currentTime);
		if (trans) { state_cm->reset_cooldown("area_attack_duration", currentTime); };
		return trans;  
	});
	state->add_transition("Waiting", "AreaAttack", [state_cm, _p_tr, &tr, dist_to_attack]() {
		return state_cm->can_use("area_attack_duration", sdlutils().virtualTimer().currTime())&& state_cm->is_player_near(_p_tr, &tr, dist_to_attack);
	});

	//patron2: cuando ataque 3 veces p1, pasa a 2 si el player no se aleja mucho
	state->add_transition("Waiting", "ShotAttack", [state_cm, _p_tr, &tr, dist_to_attack]() {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();

		bool trans = state_cm->can_use("shot_attack", currentTime)&& state_cm->is_player_near(_p_tr, &tr, dist_to_attack);
		if (trans) { state_cm->reset_cooldown("shot_attack", currentTime); };
		return trans;
	});

	state->add_transition("ShotAttack", "Waiting", [state_cm, _p_tr, &tr, dist_to_attack]() {return true; });

	//spawn
	state->add_transition("Waiting", "SpawnMichi", [state_cm]() {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();
		bool trans = state_cm->can_use("spawn_michi", currentTime);
		if (trans) {
			state_cm->reset_cooldown("spawn_michi", currentTime);
		}
		return trans;
	});
	state->add_transition("SpawnMichi", "Waiting", [state_cm]() {return true; });

	//patron 3
	state->add_transition("Waiting", "LargeAreaAttack", [state_cm, _p_tr, &tr]() {
		uint32_t currentTime = sdlutils().virtualTimer().currTime();
		bool trans = state_cm->can_use("large_area_attack_duration", currentTime)&& state_cm->is_player_near(_p_tr, &tr, 2.0f);;
		if (trans) {
			state_cm->reset_cooldown("large_area_attack_duration", currentTime);
		}
		return trans;
	});

	state->add_transition("LargeAreaAttack", "Waiting", []() {return true;});


	// A walking si el player se aleja
	state->add_transition("Waiting", "Walking", [state_cm, _p_tr, &tr, dist_to_attack]() {
		return !state_cm->is_player_near(_p_tr, &tr, dist_to_attack);
	});

	// Estado inicial
	state->set_initial_state("Walking");
}


void GameScene::add_transition(StateMachine& state, const std::string& from, const std::string& to, const std::function<bool()>& condition) {
	state.add_transition(from, to, [condition]() {
		return condition();
	});
}

void 
GameScene::spawn_catkuza(Vector2D posVec) {
	auto&& manager = *Game::Instance()->get_mngr();
	auto&& weapon = *new WeaponCatKuza();
	auto&& tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);

	float randSize = float(sdlutils().rand().nextInt(6, 10)) / 10.0f;
	auto&& rect = *new rect_component{ 0, 0,  1.5f * randSize, 2.0f * randSize };
	auto &&rigidbody = *new rigidbody_component{rect_f32{{0.0f, -0.15f}, {0.5f, 0.6f}}, mass_f32{3.0f}, 0.05f};

	ecs::entity_t e = create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		&tr,
		&rect,
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at("catkuza"),
			tr
		),
		new Health(2),
		&weapon,
		&rigidbody
	);

	auto&& mc = *manager.addExistingComponent<MovementController>(e, new MovementController(0.05f, 5.0f, 20.0 * deccel_spawned_creatures_multi));

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr

	auto&& state = *manager.addComponent<StateMachine>(e);
	auto state_cm = state.getConditionManager();

	state_cm->set_cooldown("wind_attack_duration", 1000);
	state_cm->set_cooldown("charging_duration", 500);
	state_cm->set_cooldown("dash_attack_duration", 1000);
	state_cm->set_cooldown("explosion_attack_duration", 800);
	state_cm->set_cooldown("delayed_slash_duration", 1200);


	state_cm->add_pattern("PATTERN_1", 4);
	state_cm->add_pattern("PATTERN_2", 4);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc);
	uint32_t a = 1000;
	auto dashState = std::make_shared<DashingState>(&tr, _p_tr, &mc);
	auto chargingState = std::make_shared<WaitingState>();

	auto windAttackState = std::make_shared<AttackingState>(
		&tr, _p_tr, &weapon,
		[&weapon, &tr, _p_tr]() { 
			Vector2D shootPos = tr.getPos(); // Posición del enemigo
			weapon.wind_attack(shootPos); 
		}
	);

	auto areaAttackState = std::make_shared<AttackingState>(
		&tr, _p_tr, &weapon,
		[&weapon, &tr]() {
			Vector2D shootPos = tr.getPos(); // Posición del enemigo
			weapon.area_attack(shootPos);
		}
	);

	auto dashAttackState = std::make_shared<AttackingState>(
		&tr, _p_tr, &weapon,
		[&weapon, &tr, _p_tr, &mc]() {
			Vector2D shootPos = tr.getPos();
			Vector2D shootDir = (_p_tr->getPos() - shootPos).normalize();

			Vector2D dash_target = _p_tr->getPos() + shootDir * 1.8;
			/*std::cout << dash_target << std::endl;
			mc.dash(dash_target, 1000);*/

			weapon.dash_attack(shootPos, dash_target);
		}
	);

	auto waitingState = std::make_shared<WaitingState>();


	//poner los estado a la state
	state.add_state("Walking", walkingState);
	state.add_state("Charging", chargingState);
	state.add_state("Dash", dashState);
	state.add_state("Dash2", dashState);
	state.add_state("WindAttack", windAttackState);
	state.add_state("WindAttack2", windAttackState);
	state.add_state("DashAttack", dashAttackState);
	state.add_state("AreaAttack", areaAttackState);
	state.add_state("Waiting", waitingState);

	//Transiciones Patrón 1
	add_transition(state, "Walking", "Charging",
		[state_cm, _p_tr, &tr, &weapon]() {
			bool trans = state_cm->is_player_near(_p_tr, &tr, 5.0f) && state_cm->get_current_pattern() == "PATTERN_1";
			if (trans ) {
				state_cm->reset_cooldown("charging_duration", sdlutils().currRealTime());
				weapon.set_player_pos(_p_tr->getPos());
			}
			return trans;
		}
	);

	add_transition(state, "Charging", "WindAttack",
		[state_cm, &weapon, _p_tr]() {
			bool trans = state_cm->can_use("charging_duration", sdlutils().currRealTime());
			if (trans) {
				state_cm->reset_cooldown("wind_attack_duration", sdlutils().currRealTime());
				weapon.set_player_pos(_p_tr->getPos());
			}
			return trans;
		}
	);

	add_transition(state, "WindAttack", "Dash",
		[state_cm, &weapon, _p_tr]() {
			bool trans = state_cm->can_use("wind_attack_duration", sdlutils().currRealTime());
			if (trans ) {
				state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
				weapon.set_player_pos(_p_tr->getPos());
			}
			return trans;
		}
	);

	add_transition(state, "Dash", "DashAttack",
		[state_cm, &weapon, _p_tr]() {
			
			state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
			weapon.set_player_pos(_p_tr->getPos());
			return true;
		}
	);

	add_transition(state, "Dash", "WindAttack2",
		[state_cm]() {
			bool trans = state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
			if (trans) {
				state_cm->reset_cooldown("wind_attack_duration", sdlutils().currRealTime());
			}
			return trans;
		}
	);

	add_transition(state, "WindAttack2", "Walking",
		[state_cm]() {
			bool trans = state_cm->can_use("wind_attack_duration", sdlutils().currRealTime());
			if (trans) {
				state_cm->reset_cooldown("wind_attack_duration", sdlutils().currRealTime());
				state_cm->switch_pattern();
			}
			return trans;
		}
	);


	// Transiciones Patrón 2
	add_transition(state, "Walking", "Dash2",
		[state_cm, _p_tr, &tr, &weapon]() {
			bool trans =state_cm->get_current_pattern() == "PATTERN_2" && state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
			if (trans) {
				state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
				weapon.set_player_pos(_p_tr->getPos());
			}
			return trans;
		}
	);

	add_transition(state, "Dash2", "AreaAttack",
		[state_cm, _p_tr, &tr, &weapon]() {
			std::cout << "PATTERN_2 AreaAttack" << std::endl;
			bool trans = state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
			if (trans) {
				state_cm->reset_cooldown("explosion_attack_duration", sdlutils().currRealTime());
				std::cout << "PATTERN_2 AreaAttack" << std::endl; 
			}
			return trans;
		}
	);
	
	add_transition(state, "AreaAttack", "DashAttack",
		[state_cm, _p_tr, &tr, &weapon]() {
			bool trans = state_cm->get_current_pattern() == "PATTERN_2" && state_cm->can_use("explosion_attack_duration", sdlutils().currRealTime());
			std::cout << "PATTERN_2 DashAttack" << std::endl;
			if (trans) {
				state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
				std::cout << "PATTERN_2 DashAttack" << std::endl; 
			}
			return trans;
		}
	);

	add_transition(state, "DashAttack", "Walking",
		[state_cm, _p_tr, &tr, &weapon]() {
			bool trans = state_cm->get_current_pattern() == "PATTERN_2" && state_cm->can_use("dash_attack_duration", sdlutils().currRealTime());
			if (trans) {
				state_cm->reset_cooldown("dash_attack_duration", sdlutils().currRealTime());
				std::cout << "PATTERN_2 DashAttack" << std::endl; 
				state_cm->switch_pattern();
			}
			return trans;
		}
	);

	// Estado inicial
	state.set_initial_state("Walking");
}


void 
GameScene::spawn_sarno_rata(Vector2D posVec)
{
	auto&& manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponSarnoRata();
	auto &&tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 1.0f);

	auto e = create_enemy(&tr, "sarno_rata", static_cast<Weapon*>(&weapon), 2, 1.125f, 1.5f);
	auto&& mc = *manager.addExistingComponent<MovementController>(e, new MovementController(0.05, 5.0f, 20.0 * deccel_spawned_creatures_multi));

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr
	
	auto state = manager.addComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon);


	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
	state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [state_cm, _p_tr, &tr]() {
        return state_cm->is_player_near(_p_tr, &tr, 1.0f);
    });

	// De: Attacking a: Walking, Condición: Jugador lejos
    state->add_transition("Attacking", "Walking", [state_cm, _p_tr, &tr]() {
        return !state_cm->is_player_near(_p_tr, &tr, 3.0f);
    });

	
    // Estado inicial
    state->set_initial_state("Walking");
}

void GameScene::spawn_michi_mafioso(Vector2D posVec)
{
	auto&& manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponMichiMafioso();
	auto &&tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);

	auto e = create_enemy(&tr, "michi_mafioso", static_cast<Weapon*>(&weapon), 2, 1.0f, 1.125f);
	auto&& mc = *manager.addExistingComponent<MovementController>(e, new MovementController(0.01, 5.0f, 20.0 * deccel_spawned_creatures_multi));

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr
	
//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto backingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc, true); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon); 
	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
    state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));
    state->add_state("Backing", std::static_pointer_cast<State>(backingState));

	float dist_to_attack=4.0f;
	float dist_to_fallback=3.0f;

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [state_cm, _p_tr, &tr, dist_to_attack]() {
        return state_cm->is_player_near(_p_tr, &tr, dist_to_attack);
    });

	// De: Attacking a: Walking, Condición: Jugador lejos
    state->add_transition("Attacking", "Walking", [state_cm, _p_tr, &tr, dist_to_attack]() {
        return !state_cm->is_player_near(_p_tr, &tr, dist_to_attack);
    });

	// De: Walking a: Backing, Condición: Jugador cerca
	state->add_transition("Walking", "Backing", [state_cm, _p_tr, &tr, dist_to_fallback]() {
		return state_cm->is_player_near(_p_tr, &tr, dist_to_fallback);
	});
	
	// De: Backing a: Walking, Condición: Jugador lejos y Jugador lejos de ataque
	state->add_transition("Backing", "Walking", [state_cm, _p_tr, &tr, dist_to_fallback, dist_to_attack]() {
		return !state_cm->is_player_near(_p_tr, &tr, dist_to_fallback) && !state_cm->is_player_near(_p_tr, &tr, dist_to_attack);
	});

	// De: Attacking a: Backing, Condición: Jugador cerca
	state->add_transition("Attacking", "Backing", [state_cm, _p_tr, &tr, dist_to_fallback]() {
		return state_cm->is_player_near(_p_tr, &tr, dist_to_fallback);
	});

	// De: Backing a: Attacking, Condición: Jugador lejos
	state->add_transition("Backing", "Attacking", [state_cm, _p_tr, &tr, dist_to_fallback]() {
		return !state_cm->is_player_near(_p_tr, &tr, dist_to_fallback);
	});

    // Estado inicial
    state->set_initial_state("Walking");
}

void GameScene::spawn_plim_plim(Vector2D posVec)
{
	auto&& manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponPlimPlim();
	auto&& tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);

	auto e = create_enemy(&tr, "plim_plim", static_cast<Weapon*>(&weapon), 2, 1.0f, 1.0f);
	auto&& mc = *manager.addExistingComponent<MovementController>(e, new MovementController(0.02, 5.0f, 20.0 * deccel_spawned_creatures_multi));

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr

//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon);
	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
    state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [state_cm, _p_tr, &tr]() {
        return state_cm->is_player_near(_p_tr, &tr, 4.0f);
    });

	// De: Attacking a: Walking, Condición: Jugador lejos
    state->add_transition("Attacking", "Walking", [state_cm, _p_tr, &tr]() {
        return !state_cm->is_player_near(_p_tr, &tr, 6.0f);
    });

    // Estado inicial
    state->set_initial_state("Walking");
}


void GameScene::spawn_boom(Vector2D posVec)
{
	auto&& manager = *Game::Instance()->get_mngr();
	auto &&weapon =* new WeaponBoom();
	auto&& tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);

	auto e = create_enemy(&tr, "boom", static_cast<Weapon*>(&weapon), 2, 1.8f, 1.8f);
	auto&& mc = *manager.addExistingComponent<MovementController>(e, new MovementController(0.08,5.0f, 20.0 * deccel_spawned_creatures_multi));

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr
	
//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon, [e]() {Game::Instance()->get_mngr()->setAlive(e, false); });

	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
    state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [state_cm, _p_tr, &tr]() {
        return state_cm->is_player_near(_p_tr, &tr, 1.0f);
    });

    // Estado inicial
    state->set_initial_state("Walking");
}

void GameScene::spawn_ratatouille(Vector2D posVec)
{
	auto&& manager = *Game::Instance()->get_mngr();
	auto&& tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);

	auto e = create_enemy(&tr, "ratatouille", nullptr, 2, 1.0f, 1.0f);
	auto&& mc = *manager.addExistingComponent<MovementController>(e, new MovementController(0.06, 5.0f, 20.0*deccel_spawned_creatures_multi));

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr

	//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e);
	auto state_cm = state->getConditionManager();

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc);
	auto rotatingState = std::make_shared<RotatingState>(&tr, _p_tr, &mc);

	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
	state->add_state("Rotating", std::static_pointer_cast<State>(rotatingState));

	float dist_to_rotate = 4.0f;

	// Condiciones de cada estado
	// De: Walking a: Rotating, Condición: Jugador cerca
	state->add_transition("Walking", "Rotating", [state_cm, _p_tr, &tr, dist_to_rotate]() {
		return state_cm->is_player_near(_p_tr, &tr, dist_to_rotate);
		});

	// De: Rotating a: Walking, Condición: Jugador lejos
	state->add_transition("Rotating", "Walking", [state_cm, _p_tr, &tr, dist_to_rotate]() {
		return !state_cm->is_player_near(_p_tr, &tr, dist_to_rotate * 2);
		});


	// Estado inicial
	state->set_initial_state("Walking");
}

void GameScene::spawn_wave_manager()
{
	auto ent = create_entity(
		ecs::grp::DEFAULT,
		ecs::scene::GAMESCENE,
		new transformless_dyn_image(
			{ {0.2,0.1},{0.6,0.2} },
			0,
			Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam,
			&sdlutils().images().at("event_letters")
		),
		new WaveManager()
	);
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::WAVE, ent);
}
void GameScene::spawn_fog()
{
	auto ent = create_entity(
		ecs::grp::DEFAULT,
		ecs::scene::GAMESCENE,
		new Fog()
	);
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::FOGGROUP, ent);
}
void GameScene::create_hud()
{
	auto ent = create_entity(
		ecs::grp::DEFAULT,
		ecs::scene::GAMESCENE,
		new HUD());
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::HUD_ENTITY, ent);
}



void GameScene::generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid)
{
	auto manager = Game::Instance()->get_mngr();
	(void)gid;
	//std::cout << std::endl << atan2(bp.dir.getY(), bp.dir.getX()) << " = " << atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI << std::endl;
	auto&& transform = *new Transform(bp.init_pos, bp.dir, (atan2(-bp.dir.getY(), bp.dir.getX()) + M_PI / 2) * 180.0f / M_PI, bp.speed);
	auto&& rect = *new rect_component{ 0, 0, bp.width, bp.height };
	//std::cout << bp.speed << std::endl;
	create_entity(
		gid,
		ecs::scene::GAMESCENE,
		&transform,
		&rect,
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager->getComponent<camera_component>(manager->getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at(bp.sprite_key),
			transform
		),
		new LifetimeTimer(bp.life_time),
		new BulletData(bp.damage, bp.weapon_type)
	);
}

void GameScene::check_collision() {
	auto* mngr = Game::Instance()->get_mngr();
	auto player = mngr->getHandler(ecs::hdlr::PLAYER);
	if (player != nullptr) {
		//player transform
		auto pTR = mngr->getComponent<Transform>(player);

		//enemy array
		auto& enemies = mngr->getEntities(ecs::grp::ENEMY);

		//player bullet array
		auto& pBullets = mngr->getEntities(ecs::grp::PLAYERBULLETS);

		//Enemy-PlayerBullet collision
		for (auto e : enemies) {
			//check if the actual enemy is alive
			if (mngr->isAlive(e)) {
				//actual enemy transform
				auto eTR = mngr->getComponent<Transform>(e);
				auto&& eRT = *mngr->getComponent<rect_component>(e);
				for (auto b : pBullets) {
					auto bTR = mngr->getComponent<Transform>(b);
					if (Collisions::collides(eTR->getPos(), eRT.rect.size.x, eRT.rect.size.y, //
						bTR->getPos(), eRT.rect.size.x, eRT.rect.size.y)) {
						int bDamage = mngr->getComponent<BulletData>(b)->damage();
						auto eHealth = mngr->getComponent<Health>(e);
						eHealth->takeDamage(bDamage);
					}
				}
			}
		}

		// EnemyBullets-Player collision
		auto& eBullets = mngr->getEntities(ecs::grp::ENEMYBULLETS);
		for (auto b : eBullets) {
			if (mngr->isAlive(b)) {
				auto bTR = mngr->getComponent<Transform>(b);
				auto&& bRT = *mngr->getComponent<rect_component>(b);
				if (Collisions::collides(pTR->getPos(), bRT.rect.size.x, bRT.rect.size.y, //
					bTR->getPos(), bRT.rect.size.x, bRT.rect.size.y)) {
					auto pHealth = mngr->getComponent<Health>(player);
					int bDamage = mngr->getComponent<BulletData>(b)->damage();
					pHealth->takeDamage(bDamage);
				}
			}
		}
	}
}

void GameScene::event_callback0(const event_system::event_receiver::Msg& m) {
	deccel_spawned_creatures_multi *= m.float_value;
}