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
	GameStructs::ButtonProperties buttonPropTemplate = { {sdlutils().width()/4.0f, sdlutils().height()/2.0f},
		200.0f, 200.0f, 0.0f, ""
	};
	GameStructs::ButtonProperties revolverB = buttonPropTemplate;
	revolverB.sprite_key = "revolver_button";
	create_weapon_button(GameStructs::REVOLVER, revolverB); 

	GameStructs::ButtonProperties rampageB = buttonPropTemplate;
	rampageB.sprite_key = "rampage_button";
	rampageB.pos.setX(sdlutils().width() /2.0f);
	create_weapon_button(GameStructs::RAMPAGE, rampageB);
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
	Game::Instance()->get_mngr()->update(ecs::scene::SELECTIONMENUSCENE, delta_time);
}

void SelectionMenuScene::render()
{
	Game::Instance()->get_mngr()->render(ecs::scene::SELECTIONMENUSCENE);
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
			//mngr->addComponent<Revolver>(player);
			std::cout << "revolver chosen" << std::endl;
			break;
		case GameStructs::RAMPAGE:
			//mngr->addComponent<Rampage>(player);
			std::cout << "rampage chosen" << std::endl;
			break;
		default:
			break;
		}
		//ir a la escena de juego
		Game::Instance()->change_Scene(Game::GAMESCENE);
	});
	buttonComp->connectHover([buttonComp]() {
	});
}
