#include "SelectionMenuScene.h"
#include "../our_scripts/components/Button.h"
#include "GameStructs.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"

#include "../our_scripts/components/Revolver.h"
#include "../our_scripts/components/Rampage.h"
#include "../our_scripts/components/PumpShotgun.h"

#include <iostream>
#include <string>


SelectionMenuScene::SelectionMenuScene(): Scene(ecs::scene::SELECTIONMENUSCENE)
{
}

SelectionMenuScene::~SelectionMenuScene()
{
}

void SelectionMenuScene::initScene() {
    GameStructs::ButtonProperties buttonPropTemplate = { {sdlutils().width() / 6.0f, sdlutils().height() / 2.0f},
        200.0f, 200.0f, 0.0f, ""
    };
    GameStructs::ButtonProperties revolverB = buttonPropTemplate;
    revolverB.sprite_key = "revolver_button";
    create_weapon_button(GameStructs::REVOLVER, revolverB);

    GameStructs::ButtonProperties rampageB = buttonPropTemplate;
    rampageB.sprite_key = "rampage_button";
    rampageB.pos.setX(sdlutils().width() / 3.0f);
    create_weapon_button(GameStructs::RAMPAGE, rampageB);

    GameStructs::ButtonProperties pump_shotgun_B = buttonPropTemplate;
    pump_shotgun_B.sprite_key = "pump_shotgun_button";
    pump_shotgun_B.pos.setX(sdlutils().width() / 1.5f);
    create_weapon_button(GameStructs::PUMP_SHOTGUN, pump_shotgun_B);
}
void SelectionMenuScene::enterScene()
{
}

void SelectionMenuScene::exitScene()
{
}


void SelectionMenuScene::create_weapon_button(GameStructs::WeaponType wt, const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto player = mngr->getHandler(ecs::hdlr::PLAYER);
    buttonComp->connectClick([buttonComp, &mngr, wt, player]() {
        if (buttonComp->clicked()) return;
        buttonComp->set_clicked(true);
        //std::cout << "left click-> button" << std::endl;

        switch (wt) {
        case GameStructs::REVOLVER:
            //std::cout << "revolver chosen" << std::endl;
            mngr->addComponent<Revolver>(player);
            break;
        case GameStructs::RAMPAGE:
            //std::cout << "rampage chosen" << std::endl;
            mngr->addComponent<Rampage>(player);
            break;
        case GameStructs::PUMP_SHOTGUN:
            mngr->addComponent<PumpShotgun>(player);
            break;
        default:
            break;
        }

        Game::Instance()->change_Scene(Game::GAMESCENE);
        });

    buttonComp->connectHover([buttonComp]() {
        (void)buttonComp;
    });
}