#include "GameScene.h"

#include "Game.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/Transform.h"
#include "../our_scripts/components/MovementController.h"
#include "../our_scripts/components/LifetimeTimer.h"
#include "../our_scripts/states/Conditions.h"
#include "../our_scripts/states/WalkingState.h"
#include "../our_scripts/states/AttackingState.h"

#include "../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/components/dyn_image.hpp"
#include "../our_scripts/components/camera_component.hpp"

#include "../our_scripts/components/Health.h" 
#include "../our_scripts/components/BulletData.h"

#include "../utils/Collisions.h" 

#include "../our_scripts/components/WaveManager.h"
#include "../our_scripts/components/WeaponMichiMafioso.h"
#include "../our_scripts/components/WeaponPlimPlim.h"
#include "../our_scripts/components/WeaponSarnoRata.h"
#include "../our_scripts/components/WeaponBoom.h"
#include "../our_scripts/components/Health.h"

#include "../our_scripts/components/EnemyStateMachine.h"
#include "../our_scripts/components/StateMachine.h"
#include "../our_scripts/components/dyn_image.hpp"
#include "../our_scripts/components/render_ordering.hpp"
#include "../our_scripts/components/rect_component.hpp"
#include "../our_scripts/components/StopOnBorder.h"
#include "../our_scripts/components/HUD.h"

#include "../our_scripts/components/render_ordering.hpp"
#include "../our_scripts/card_system/PlayableCards.hpp"
#include "../our_scripts/card_system/CardUpgrade.hpp"

#include <iostream>
#include <string>


GameScene::GameScene() : Scene(ecs::scene::GAMESCENE){}

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

	create_environment();
	spawn_wave_manager();
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
}

void GameScene::exitScene()
{
}

ecs::entity_t GameScene::create_player()
{
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&camera = manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam;
	
	auto &&player_transform = *new Transform({ 0.0f, 0.0f }, { 0.0f,0.0f }, 0.0f, 2.0f);
	auto &&player_rect = *new rect_component{0, 0, 1.5f, 2.0f};
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
		new MovementController(0.1f,5.0f,20.0f),
		new StopOnBorder(camera, 1.5f, 2.0f),
		new HUD()
		);
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::PLAYER, player);
	return player;
}


ecs::entity_t 
GameScene::create_enemy(Transform* tr, const std::string& spriteKey, Weapon* weapon, float health, float width, float height){
	auto&& manager = *Game::Instance()->get_mngr();

	float randSize = float(sdlutils().rand().nextInt(6, 14)) / 10.0f;
	auto&& rect = *new rect_component{ 0, 0, width * randSize, height * randSize };
	auto e = create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		tr,
		&rect,
		new MovementController(),
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at(spriteKey),
			*tr
		),
		new Health(health),
		weapon
	);

	return e;
}

void GameScene::spawn_sarno_rata(Vector2D posVec)
{
	auto&& manager = *Game::Instance()->get_mngr();
	auto &&weapon = *new WeaponSarnoRata();
	auto &&tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);

	auto e = create_enemy(&tr, "sarno_rata", static_cast<Weapon*>(&weapon), 2, 1.5f, 2.0f);
	auto&& mc = *manager.getComponent<MovementController>(e);


	ConditionManager conditionManager;

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr
	
	auto state = manager.addComponent<StateMachine>(e, conditionManager);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon); 
	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
    state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [&conditionManager, _p_tr, &tr]() {
        return conditionManager.isPlayerNear(_p_tr, &tr, 5.0f);  
    });

	// De: Attacking a: Walking, Condición: Jugador lejos
    state->add_transition("Attacking", "Walking", [&conditionManager, _p_tr, &tr]() {
        return !conditionManager.isPlayerNear(_p_tr, &tr, 5.0f); 
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
	auto&& mc = *manager.getComponent<MovementController>(e);

	ConditionManager conditionManager;

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr
	
//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e, conditionManager);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto backingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc, true); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon); 
	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
    state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));
    state->add_state("Backing", std::static_pointer_cast<State>(backingState));

	float dist_to_attack=10.0f;
	float dist_to_fallback=6.0f;

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [&conditionManager, _p_tr, &tr, dist_to_attack]() {
        return conditionManager.isPlayerNear(_p_tr, &tr, dist_to_attack);  
    });

	// De: Attacking a: Walking, Condición: Jugador lejos
    state->add_transition("Attacking", "Walking", [&conditionManager, _p_tr, &tr, dist_to_attack]() {
        return !conditionManager.isPlayerNear(_p_tr, &tr, dist_to_attack); 
    });

	// De: Walking a: Backing, Condición: Jugador cerca
	state->add_transition("Walking", "Backing", [&conditionManager, _p_tr, &tr, dist_to_fallback]() {
		return conditionManager.isPlayerNear(_p_tr, &tr, dist_to_fallback);  
	});
	
	// De: Backing a: Walking, Condición: Jugador lejos y Jugador lejos de ataque
	state->add_transition("Backing", "Walking", [&conditionManager, _p_tr, &tr, dist_to_fallback, dist_to_attack]() {
		return !conditionManager.isPlayerNear(_p_tr, &tr, dist_to_fallback) && !conditionManager.isPlayerNear(_p_tr, &tr, dist_to_attack);  
	});

	// De: Attacking a: Backing, Condición: Jugador cerca
	state->add_transition("Attacking", "Backing", [&conditionManager, _p_tr, &tr, dist_to_fallback]() {
		return conditionManager.isPlayerNear(_p_tr, &tr, dist_to_fallback);  
	});

	// De: Backing a: Attacking, Condición: Jugador lejos
	state->add_transition("Backing", "Attacking", [&conditionManager, _p_tr, &tr, dist_to_fallback]() {
		return !conditionManager.isPlayerNear(_p_tr, &tr, dist_to_fallback);  
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
	auto&& mc = *manager.getComponent<MovementController>(e);

	ConditionManager conditionManager;

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr

//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e, conditionManager);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon); 
	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
    state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [&conditionManager, _p_tr, &tr]() {
        return conditionManager.isPlayerNear(_p_tr, &tr, 5.0f);  
    });

	// De: Attacking a: Walking, Condición: Jugador lejos
    state->add_transition("Attacking", "Walking", [&conditionManager, _p_tr, &tr]() {
        return !conditionManager.isPlayerNear(_p_tr, &tr, 10.0f); 
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
	auto&& mc = *manager.getComponent<MovementController>(e);


	ConditionManager conditionManager;

	auto playerEntities = manager.getEntities(ecs::grp::PLAYER);

	Transform* _p_tr = manager.getComponent<Transform>(playerEntities[0]); // el primero por ahr
	
//	StateMachine(ConditionManager& conditionManager, Transform* playerTransform, Transform* enemyTransform, float dist);
	auto state = manager.addComponent<StateMachine>(e, conditionManager);

	// Crear estados
	auto walkingState = std::make_shared<WalkingState>(&tr, _p_tr, &mc); 
	auto attackingState = std::make_shared<AttackingState>(&tr, _p_tr, &weapon, [e]() {Game::Instance()->get_mngr()->setAlive(e, false); });

	//poner los estado a la state
	state->add_state("Walking", std::static_pointer_cast<State>(walkingState));
    state->add_state("Attacking", std::static_pointer_cast<State>(attackingState));

    // Condiciones de cada estado
	// De: Walking a: Attacking, Condición: Jugador cerca
    state->add_transition("Walking", "Attacking", [&conditionManager, _p_tr, &tr]() {
        return conditionManager.isPlayerNear(_p_tr, &tr, 1.0f);  
    });

    // Estado inicial
    state->set_initial_state("Walking");
}


void GameScene::spawn_wave_manager()
{
	create_entity(
		ecs::hdlr::WAVE,
		ecs::scene::GAMESCENE,
		new WaveManager()
	);
}

void GameScene::generate_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid)
{
	auto manager = Game::Instance()->get_mngr();
	(void)gid;
	//std::cout << std::endl << atan2(bp.dir.getY(), bp.dir.getX()) << " = " << atan2(bp.dir.getY(), bp.dir.getX()) * 180.0f / M_PI << std::endl;
	auto &&transform = *new Transform(bp.init_pos, bp.dir, (atan2(-bp.dir.getY(), bp.dir.getX())+M_PI/2) * 180.0f / M_PI, bp.speed);
	auto &&rect = *new rect_component{0, 0, bp.width, bp.height};
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
				auto &&eRT = *mngr->getComponent<rect_component>(e);
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
				auto &&bRT = *mngr->getComponent<rect_component>(b);
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