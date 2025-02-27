#include "SelectionMenuScene.h"
#include "../our_scripts/components/Button.h"
#include "GameStructs.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"

#include "../our_scripts/components/Revolver.h"
#include "../our_scripts/components/Rampage.h"

#include <iostream>
#include <string>

SelectionMenuScene::SelectionMenuScene(): Scene(ecs::scene::SELECTIONMENUSCENE)
{
	GameStructs::ButtonProperties buttonPropTemplate = { {sdlutils().width()/4, sdlutils().height()/2},
		50.0f, 50.0f, 0.0f, ""
	};
	GameStructs::ButtonProperties revolverB = buttonPropTemplate;
	revolverB.sprite_key = "revolver";
	create_weapon_button(GameStructs::REVOLVER, revolverB); 

	GameStructs::ButtonProperties rampageB = buttonPropTemplate;
	rampageB.sprite_key = "rampage";
	rampageB.pos.setX(sdlutils().width() * 3 / 4);
	create_weapon_button(GameStructs::RAMPAGE, revolverB);
}

SelectionMenuScene::~SelectionMenuScene()
{
}

void SelectionMenuScene::initScene() {

}
void SelectionMenuScene::enterScene()
{
}

void SelectionMenuScene::exitScene()
{
}

void SelectionMenuScene::update(uint32_t delta_time)
{
	Game::Instance()->get_mngr()->update(ecs::scene::GAMESCENE, delta_time);
}

void SelectionMenuScene::render()
{
	Game::Instance()->get_mngr()->render(ecs::scene::GAMESCENE);
}
void 
SelectionMenuScene::create_weapon_button(GameStructs::WeaponType wt, const GameStructs::ButtonProperties& bp) {
	auto* mngr = Game::Instance()->get_mngr();
	auto e = create_button(bp);
	auto buttonComp = mngr->getComponent<Button>(e);
	//add callbacks
	buttonComp->connectClick([buttonComp, &mngr, wt]() {
		std::cout << "left click-> button" << std::endl;
		//añadir el tipo de arma
		auto player = mngr->getHandler(ecs::hdlr::PLAYER);
		//REMEMBER TO INITIALIZE THIS COMPONENT IS GAMESTATE::ENTER()
		switch (wt)
		{
		case GameStructs::REVOLVER:
			mngr->addComponent<Revolver>(player);
			break;
		case GameStructs::RAMPAGE:
			mngr->addComponent<Rampage>(player);
			break;
		default:
			break;
		}
		//ir a la escena de juego
		Game::Instance()->change_Scene(Game::GAMESCENE);
	});
	buttonComp->connectHover([buttonComp]() {
		std::cout << "hover-> button" << std::endl;
	});
}
