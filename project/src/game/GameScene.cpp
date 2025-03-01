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

#include "../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../our_scripts/components/Revolver.h"
#include "../our_scripts/components/Rampage.h"
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
#include "../our_scripts/components/dyn_image.hpp"
#include "../our_scripts/components/render_ordering.hpp"
#include "../our_scripts/components/rect_component.hpp"

#include "../our_scripts/components/render_ordering.hpp"


#include <iostream>
#include <string>

GameScene::GameScene() : Scene(ecs::scene::GAMESCENE), _player(nullptr)
{}

static ecs::entity_t create_environment() {
	auto&& manager = *Game::Instance()->get_mngr();
	auto environment = manager.addEntity(ecs::scene::GAMESCENE);
	auto &&tr = *manager.addComponent<Transform>(environment, Vector2D(-16.0, 9.0), Vector2D(0.0, 0.0), 0.0f, 0.05f);
	auto &&rect = *manager.addComponent<rect_component>(environment, 0.0f, 0.0f, 32.0f, 18.0f);
	(void)tr;
	manager.addComponent<offset_dyn_image>(environment, rect_f32{
		{0.0, 0.0},
		{1.0, 1.0}
	}, rect, manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam, sdlutils().images().at("floor"), tr);
	manager.addComponent<render_ordering>(environment, 0);
	return environment;
}


void GameScene::initScene() {
	const rendering::camera_creation_descriptor_flags flags =
		rendering::camera_creation_descriptor_options::camera_creation_descriptor_options_set_handler
		| rendering::camera_creation_descriptor_options::camera_creation_descriptor_options_clamp;
	ecs::entity_t camera = rendering::create_camera(ecs::scene::GAMESCENE, flags, nullptr);
	ecs::entity_t player = spawnPlayer();

	auto &&manager = *Game::Instance()->get_mngr();
	manager.addComponent<camera_follow>(camera, camera_follow_descriptor{
		.previous_position = manager.getComponent<camera_component>(camera)->cam.camera.position,
		.lookahead_time = 1.0f,
		.semi_reach_time = 2.5f
	}, *manager.getComponent<camera_component>(camera), *manager.getComponent<Transform>(player));

	create_environment();
	spawnWaveManager();
}

void GameScene::enterScene()
{
	auto* mngr = Game::Instance()->get_mngr();
	auto player = mngr->getHandler(ecs::hdlr::PLAYER);
	auto w = mngr->getComponent<Weapon>(player);

	w->initComponent();
	mngr->addComponent<KeyboardPlayerCtrl>(player);
}

void GameScene::exitScene()
{
}

ecs::entity_t GameScene::spawnPlayer()
{
	std::list<Card*> c = { new Fireball(), new Minigun(), new Lighting(), new Minigun() };
	auto &&manager = *Game::Instance()->get_mngr();
	auto &&camera = manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam;
	
	auto &&player_transform = *new Transform({ 0.0f, 0.0f }, { 0.0f,0.0f }, 0.0f, 2.0f);
	auto &&player_rect = *new rect_component{0, 0, 1.75f, 2.0f};
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
		new MovementController(),
		new Deck(c)
		);
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::PLAYER, player);
	return player;
}


void GameScene::spawnSarnoRata(Vector2D posVec)
{
	auto* weapon = new WeaponSarnoRata();
	auto &&tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);
	auto &&rect = *new rect_component{0, 0, 1.5f, 2.0f};
	auto manager = Game::Instance()->get_mngr();

	//std::cout << posVec << std::endl;
	auto e = create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		&tr,
		&rect,
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager->getComponent<camera_component>(manager->getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at("sarno_rata"),
			tr
		),
		new Health(2),
		weapon
	);
	auto state = manager->addComponent<EnemyStateMachine>(e, 3);
	state->setState(EnemyStateMachine::StateType::WALKING);
}

void GameScene::spawnMichiMafioso(Vector2D posVec)
{
	auto* weapon = new WeaponMichiMafioso();
	auto &&tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);
	auto &&rect = *new rect_component{0, 0, 1.0f, 1.0f};
	auto manager = Game::Instance()->get_mngr();

	auto e = create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		&tr,
		&rect,
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager->getComponent<camera_component>(manager->getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at("michi_mafioso"),
			tr
		),
		new Health(2),
		weapon
	);

	auto state = manager->addComponent<EnemyStateMachine>(e, 3);
	state->setState(EnemyStateMachine::StateType::WALKING);
}

void GameScene::spawnPlimPlim(Vector2D posVec)
{
	auto &&tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);
	auto &&rect = *new rect_component{0, 0, 1.0f, 1.0f};
	auto* weapon = new WeaponPlimPlim();
	auto manager = Game::Instance()->get_mngr();

	auto e = create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		&tr,
		&rect,
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager->getComponent<camera_component>(manager->getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at("plim_plim"),
			tr
		),
		new Health(2),
		weapon
	);

	auto state = manager->addComponent<EnemyStateMachine>(e, 3);
	state->setState(EnemyStateMachine::StateType::WALKING);
}

void GameScene::spawnBoom(Vector2D posVec)
{
	auto &&tr = *new Transform(posVec, { 0.0f,0.0f }, 0.0f, 2.0f);
	auto &&rect = *new rect_component{0, 0, 1.0f, 1.0f};
	auto* weapon = new WeaponBoom();
	auto manager = Game::Instance()->get_mngr();

	auto e = create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		&tr,
		&rect,
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			rect,
			manager->getComponent<camera_component>(manager->getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at("boom"),
			tr
		),
		new Health(2),
		weapon
	);

	auto state = manager->addComponent<EnemyStateMachine>(e, 3);
	state->setState(EnemyStateMachine::StateType::WALKING);
}


void GameScene::spawnWaveManager()
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

	auto &&transform = *new Transform(bp.init_pos, bp.dir, bp.rot, bp.speed);
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
		new BulletData(bp.damage)
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