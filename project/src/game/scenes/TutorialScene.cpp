#include "TutorialScene.h"

#include "../../our_scripts/components/ui/Button.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "ecs/Manager.h"
#include "game/Game.h"
#include "game/scenes/GameScene.h"

#include "../../our_scripts/components/rendering/camera_component.hpp"
#include "../../our_scripts/components/rendering/transformless_dyn_image.h"
#include "../../our_scripts/components/WaveManager.h"
#include "../../our_scripts/components/KeyboardPlayerCtrl.h"
#include "../../our_scripts/components/cards/Deck.hpp"
#include "../../our_scripts/components/ui/PlayerHUD.h"
#include "../../our_scripts/components/rendering/ImageForButton.h"
#include "../../our_scripts/components/id_component.h"
#include "../../our_scripts/components/weapons/player/Revolver.h"
#include "../../our_scripts/card_system/PlayableCards.hpp"
#include "../../our_scripts/components/Health.h"

#include <iostream>

TutorialScene::TutorialScene()
	: Scene(ecs::scene::TUTORIALSCENE),
	_current_pop_up(0), _current_pop_up_entity(nullptr), 
	_popup_timer(0), _tutorial_state(TutorialState::NONE), _player_health() {

	event_system::event_manager::Instance()->suscribe_to_event(event_system::enemy_dead, this, &event_system::event_receiver::event_callback0);

	GameScene::create_environment(ecs::scene::TUTORIALSCENE);
	GameStructs::ButtonProperties mainMenuB = { {{ 0.025, 0.02f }, {0.1f, 0.08f}  }, 0.0f, "boton_mainmenu_tutorial" };
	create_change_scene_button(mainMenuB, Game::MAINMENU);

	_pop_ups.push_back({ "popup_move",
		[]() {
			auto& input = *InputHandler::Instance();
			return (input.isKeyDown(SDL_SCANCODE_W) || input.isKeyDown(SDL_SCANCODE_A) ||
				input.isKeyDown(SDL_SCANCODE_S) || input.isKeyDown(SDL_SCANCODE_D))
				||input.getRStick().getX() != 0.0f|| input.getRStick().getY() != 0.0f;
		},
		3000,{{0.1f, 0.1f}, {0.3f, 0.3f}}
		});

	_pop_ups.push_back({ "pop_up_player_hud",
		[this]() { return has_pass_input(); },
		200,{{0.0f, 0.0f}, {1.0f, 1.0f}},
		[]() {
			auto* mngr = Game::Instance()->get_mngr();
			auto player = mngr->getHandler(ecs::hdlr::TUTORIALPLAYER);
			mngr->addComponent<PlayerHUD>(player);
		}
	});

	_pop_ups.push_back({ "popup_use_card",
		[]() {
			auto& input = *InputHandler::Instance();
			bool LT= input.isControllerButtonDown(InputHandler::CONTROLLER_BUTTONS::LT);
			if (LT)input.consume(InputHandler::CONTROLLER_BUTTONS::LT);
			return (input.mouseButtonDownEvent() && input.getMouseButtonState(InputHandler::RIGHT)) || LT;
		},
		2000,{{0.1f, 0.1f}, {0.35f, 0.35f}},
		});

	_pop_ups.push_back({ "popup_reload",
		[]() {
			auto& input = *InputHandler::Instance();
			bool B = input.isControllerButtonDown(InputHandler::CONTROLLER_BUTTONS::B);
			if (B)input.consume(InputHandler::CONTROLLER_BUTTONS::B);
			return input.isKeyDown(SDL_SCANCODE_SPACE) || B; },
		2000, {{0.1f, 0.1f}, {0.35f, 0.35f}}
	});


	_pop_ups.push_back({ "popup_attack",
		[]() {
			auto& input = *InputHandler::Instance();
			bool RT = input.isControllerButtonDown(InputHandler::CONTROLLER_BUTTONS::RT);
			if (RT)input.consume(InputHandler::CONTROLLER_BUTTONS::RT);
			return (input.mouseButtonDownEvent() && input.getMouseButtonState(InputHandler::LEFT)) || RT;
		},
		3000, {{0.1f, 0.1f}, {0.35f, 0.35f}}
	});

	_pop_ups.push_back({ "popup_enemy",
		[this]() {
			return _enemy_killed;
		},
		2000, {{0.6f, 0.6f}, {0.35f, 0.35f}},
		[]() {GameScene::spawn_plim_plim({ 0.0f, 1.0f },ecs::scene::TUTORIALSCENE);}
	});
	_pop_ups.push_back({ "popup_oleada",
		[this]() { return has_pass_input(); },
		200, {{0.0f, 0.0f}, {1.0f, 1.0f}},
		[]() {
			auto hud = GameScene::create_hud(ecs::scene::TUTORIALSCENE);
			Game::Instance()->get_mngr()->setHandler(ecs::hdlr::TURORIALHUD, hud);
		}
	});
	_pop_ups.push_back({ "popup_objetos_miticos",
		[this]() { return has_pass_input(); },
		250, {{0.05f, 0.05f}, {0.85f, 0.85f}},
	});
	_pop_ups.push_back({ "popup_recompensa",
		[this]() { return has_pass_input(); },
		250, {{0.05f, 0.05f}, {0.85f, 0.85f}},
	});
	_pop_ups.push_back({ "popup_ganar",
		[this]() { return has_pass_input(); },
		50, {{0.0f, 0.0f}, {1.0f, 1.0f}},
	});

}	

TutorialScene::~TutorialScene()
{
	event_system::event_manager::Instance()->unsuscribe_to_event(event_system::change_deccel, this, &event_system::event_receiver::event_callback0);
}

void TutorialScene::initScene() {
	ecs::entity_t player = GameScene::create_player(ecs::scene::TUTORIALSCENE);
	Game::Instance()->get_mngr()->setHandler(ecs::hdlr::TUTORIALPLAYER, player);

	auto* mngr = Game::Instance()->get_mngr();
	mngr->addComponent<Revolver>(player)->initComponent();

	mngr->addComponent<id_component>(player);
	mngr->addComponent<MythicComponent>(player);

	std::list<Card*> cl = { new Fireball(), new Lighting(), new Minigun() };
	mngr->addComponent<Deck>(player, cl);
	mngr->addComponent<KeyboardPlayerCtrl>(player);

	_player_health = mngr->getComponent<Health>(player);
}
void TutorialScene::enterScene()
{
	Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::WAVE));
	auto camera = Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA);
	Game::Instance()->get_mngr()->change_ent_scene(camera, ecs::scene::TUTORIALSCENE);
	_tutorial_state = TutorialState::NEXT_POP_UP;

	auto&& manager = *Game::Instance()->get_mngr();
	ecs::entity_t player = manager.getHandler(ecs::hdlr::TUTORIALPLAYER);

	manager.addComponent<camera_follow>(camera, camera_follow_descriptor{
		.previous_position = manager.getComponent<camera_component>(camera)->cam.camera.position,
		.lookahead_time = 1.0f,
		.semi_reach_time = 2.5f
		}, *manager.getComponent<camera_component>(camera), *manager.getComponent<Transform>(player));


	if(manager.hasComponent<PlayerHUD>(player))	manager.removeComponent<PlayerHUD>(player);
	auto hud = manager.getHandler(ecs::hdlr::TURORIALHUD);
	if (hud) manager.setAlive(hud,false);

	manager.refresh();
}

void TutorialScene::exitScene()
{
	if (_current_pop_up_entity)  Game::Instance()->get_mngr()->setAlive(_current_pop_up_entity, false);
	_current_pop_up = 0;
	_enemy_killed = false;
}
void TutorialScene::update(uint32_t delta_time) {
	Scene::update(delta_time);

	auto* mngr = Game::Instance()->get_mngr();
	if (_player_health->getHealth() < 10) _player_health->resetCurrentHeatlh(); //para que el player no muera
	switch (_tutorial_state)
	{
	case TutorialState::NEXT_POP_UP:
		if (_current_pop_up >= _pop_ups.size()) {
			_tutorial_state = TutorialState::FINISHED;
		}
		else
		{
			create_pop_up();
			_popup_timer = 0;
			_tutorial_state = TutorialState::WAIT_FOR_ACTION;
		}
		break;
	case TutorialState::WAIT_FOR_ACTION:
		if (_pop_ups[_current_pop_up].condition()) {
			_tutorial_state = TutorialState::WAIT_FOR_DURATION;
		}
		break;
	case TutorialState::WAIT_FOR_DURATION:
		_popup_timer += delta_time;
		if (_popup_timer >= _pop_ups[_current_pop_up].duration) {
			mngr->setAlive(_current_pop_up_entity, false);
			_current_pop_up_entity = nullptr;
			_current_pop_up++;
			_tutorial_state = TutorialState::NEXT_POP_UP;
		}
		break;
	case TutorialState::FINISHED:
		Game::Instance()->change_Scene(Game::SELECTIONMENU);
		break;
	default:
		break;
	}


}

void TutorialScene::create_pop_up() {
	auto* mngr = Game::Instance()->get_mngr();
	auto& cam = mngr->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam;
	auto& current = _pop_ups[_current_pop_up];
	_current_pop_up_entity = create_entity(
		ecs::grp::TUTORIALPOPUPS,
		ecs::scene::TUTORIALSCENE,
		new transformless_dyn_image(current.rect, 0, cam, &sdlutils().images().at(current.image_key))
	);
	if (current.on_enter)current.on_enter();
}

void TutorialScene::create_proyectile(const GameStructs::BulletProperties& bp, ecs::grpId_t gid)
{
	GameScene::generate_proyectile(bp, gid, ecs::scene::TUTORIALSCENE);
}

void TutorialScene::event_callback0(const event_system::event_receiver::Msg& m)
{
	_enemy_killed = true;
}

bool TutorialScene::has_pass_input()
{
	auto& input = *InputHandler::Instance();
	bool A = input.isControllerButtonDown(InputHandler::CONTROLLER_BUTTONS::A);
	if (A)input.consume(InputHandler::CONTROLLER_BUTTONS::A);
	return input.isKeyDown(SDL_SCANCODE_F) || A;
}

void TutorialScene::create_change_scene_button(const GameStructs::ButtonProperties& bp, Game::State nextScene)
{
	auto* mngr = Game::Instance()->get_mngr();
	auto e = create_button(bp);
	auto buttonComp = mngr->getComponent<Button>(e);

	auto imgComp = mngr->addComponent<ImageForButton>(e,
		&sdlutils().images().at(bp.sprite_key),
		&sdlutils().images().at(bp.sprite_key),
		bp.rect,
		0,
		Game::Instance()->get_mngr()->getComponent<camera_component>(
			Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
	);
	buttonComp->connectClick([buttonComp, &mngr, nextScene]() {
		Game::Instance()->change_Scene(nextScene);
		});

	buttonComp->connectHover([buttonComp, imgComp, this]() { imgComp->_filter = true; });
	buttonComp->connectExit([buttonComp, imgComp, this]() { imgComp->_filter = false; });
}