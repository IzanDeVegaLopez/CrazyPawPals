#include "GameScene.h"

#include "Game.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/Transform.h"
#include "../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../our_scripts/components/MovementController.h"
#include "../our_scripts/components/LifetimeTimer.h"

#include "../our_scripts/components/Revolver.h"
#include "../our_scripts/components/Rampage.h"
#include "../our_scripts/components/SimpleMove.h"
#include "../our_scripts/components/Mana.h"
#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/components/dyn_image.hpp"
#include "../our_scripts/components/camera_component.hpp"

#include "../our_scripts/components/Health.h" 
#include "../our_scripts//components//BulletData.h"

#include "../utils/Collisions.h" 

#include "../our_scripts/components/WaveManager.h"
#include "../our_scripts/components/WeaponMichiMafioso.h"
#include "../our_scripts/components/WeaponPlimPlim.h"
#include "../our_scripts/components/WeaponSarnoRata.h"
#include "../our_scripts/components/WeaponBoom.h"
#include "../our_scripts/components/Health.h"

#include "../our_scripts/components/EnemyStatemachine.h"
#include "../our_scripts/components/dyn_image.hpp"

#include <iostream>
#include <string>

GameScene::GameScene()
{

}

void GameScene::initScene()
{
#pragma region Bullets
	//std::vector<Bullet*> b;

#pragma endregion

#pragma region Enemies
	spawnWaveManager();

#pragma endregion

#pragma region Player

	//spawnPlayer();
	spawnSarnoRata({ sdlutils().width() / 2.0f,0});

#pragma endregion Deck

	//Deck deck = Deck(std::list<Card*>{new Card("1"), new Card("2"), new Card("3"), new Card("4")});
	////cout << deck << endl;
	//deck.add_card_to_deck(new Fireball());
	//deck.add_card_to_deck(new Minigun());

	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.use_card();
	//deck.reload();

	////deck.addCardToDeck(new Card("5"));
	//cout << deck << endl;*/
}

void GameScene::enterScene()
{
}

void GameScene::exitScene()
{
}

void GameScene::update(uint32_t delta_time)
{
	Game::Instance()->get_mngr()->update(ecs::scene::GAMESCENE,delta_time);
}

void GameScene::render()
{
	Game::Instance()->get_mngr()->render(ecs::scene::GAMESCENE);
}

void GameScene::spawnPlayer()
{
	//auto* revolver = new Revolver();
	auto* revolver = new Rampage();
	std::list<Card*> c = { new Fireball(), new Minigun(), new Lighting(), new Minigun() };
	create_entity(
		ecs::grp::PLAYER,
		ecs::scene::GAMESCENE,
		new Transform({ sdlutils().width() / 2.0f, sdlutils().height() / 2.0f }, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("player")),
		revolver,
		new Health(100),
		new ManaComponent(),
		new Deck(c),
		new MovementController(),
		new KeyboardPlayerCtrl()
		);
	revolver->initComponent();
	revolver->set_attack_size(10, 10);
}

void GameScene::spawnSarnoRata(Vector2D posVec)
{
	auto* weapon = new WeaponSarnoRata();

	create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),	
		new Health(2),
		weapon,
		new EnemyStateMachine(3)
	);
}

void GameScene::spawnMichiMafioso(Vector2D posVec)
{
	auto* weapon = new WeaponMichiMafioso();

	create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),
		new Health(2),
		weapon
	);
}

void GameScene::spawnPlimPlim(Vector2D posVec)
{
	auto* weapon = new WeaponPlimPlim();

	create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),
		new Health(2),
		weapon
	);
}

void GameScene::spawnBoom(Vector2D posVec)
{
	auto* weapon = new WeaponBoom();

	create_entity(
		ecs::grp::ENEMY,
		ecs::scene::GAMESCENE,
		new Transform(posVec, { 0.0f,0.0f }, 100.0f, 100.0f, 0.0f, 2.0f),
		new Image(&sdlutils().images().at("enemy")),
		new Health(2),
		weapon
	);
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
	//std::cout << bp.speed << std::endl;
	create_entity(
		gid,
		ecs::scene::GAMESCENE,
		new Transform(bp.init_pos, bp.dir, bp.width, bp.height, bp.rot, bp.speed),
		new dyn_image(
			rect_f32{ {0,0},{1,1} },
			size2_f32{1,1},
			manager->getComponent<camera_component>(manager->getHandler(ecs::hdlr::CAMERA))->cam,
			sdlutils().images().at(bp.sprite_key)
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
		for (auto e : enemies){
			//check if the actual enemy is alive
			if (mngr->isAlive(e)) {
				//actual enemy transform
				auto eTR = mngr->getComponent<Transform>(e);
				for (auto b : pBullets) {
					auto bTR = mngr->getComponent<Transform>(b);
					if (Collisions::collides(eTR->getPos(), eTR->getWidth(), eTR->getHeight(), //
						bTR->getPos(), bTR->getWidth(), bTR->getHeight())) {
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
				if (Collisions::collides(pTR->getPos(), pTR->getWidth(), pTR->getHeight(), //
					bTR->getPos(), bTR->getWidth(), bTR->getHeight())) {
					auto pHealth = mngr->getComponent<Health>(player);
					int bDamage = mngr->getComponent<BulletData>(b)->damage();
					pHealth->takeDamage(bDamage);
				}
			}
		}

	}
}