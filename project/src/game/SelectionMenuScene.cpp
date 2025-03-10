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
#include "../our_scripts/components/RampCanon.h"
#include "../our_scripts/components/Lightbringer.h"

#include "../our_scripts/card_system/Card.hpp"
#include "../our_scripts/card_system/PlayableCards.hpp"
#include "../our_scripts/components/Deck.hpp"
#include <iostream>
#include <string>
#include <list>


SelectionMenuScene::SelectionMenuScene(): Scene(ecs::scene::SELECTIONMENUSCENE)
{
}

SelectionMenuScene::~SelectionMenuScene()
{
}
void SelectionMenuScene::create_weapon_buttons() {
    int i = 0;
    float thrust = 0.175f;
    float h = sdlutils().height() * thrust * i++;
    GameStructs::ButtonProperties buttonPropTemplate = { {sdlutils().width() * 1.2f, h},
        100.0f, 100.0f, 0.0f, ""
    };
    GameStructs::ButtonProperties revolverB = buttonPropTemplate;
    revolverB.sprite_key = "revolver_button";
    create_weapon_button(GameStructs::REVOLVER, revolverB);

    GameStructs::ButtonProperties rampageB = buttonPropTemplate;
    rampageB.sprite_key = "rampage_button";
    h = sdlutils().height() * thrust * i++;
    rampageB.pos.setY(h);
    create_weapon_button(GameStructs::RAMPAGE, rampageB);

    GameStructs::ButtonProperties pump_shotgun_B = buttonPropTemplate;
    pump_shotgun_B.sprite_key = "pump_shotgun_button";
    h = sdlutils().height() * thrust * i++;
    pump_shotgun_B.pos.setY(h);
    create_weapon_button(GameStructs::PUMP_SHOTGUN, pump_shotgun_B);

    GameStructs::ButtonProperties ramp_canon_B = buttonPropTemplate;
    ramp_canon_B.sprite_key = "pump_shotgun_button";
    h = sdlutils().height() * thrust * i++;
    ramp_canon_B.pos.setY(h);
    create_weapon_button(GameStructs::RAMP_CANON, ramp_canon_B);

    GameStructs::ButtonProperties lightbringerB = buttonPropTemplate;
    lightbringerB.sprite_key = "lightbringer_button";
    h = sdlutils().height() * thrust * i++;
    lightbringerB.pos.setY(h);
    create_weapon_button(GameStructs::LIGHTBRINGER, lightbringerB);
}
void SelectionMenuScene::initScene() {
    _selection = &sdlutils().images().at("selection");
    create_weapon_buttons();
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
        case GameStructs::RAMP_CANON:
            mngr->addComponent<RampCanon>(player);
            break;
        case GameStructs::LIGHTBRINGER:
            mngr->addComponent<Lightbringer>(player);
            break;
        default:
            break;
        }

        //Game::Instance()->change_Scene(Game::GAMESCENE);
        });

    buttonComp->connectHover([buttonComp]() {
        (void)buttonComp;
    });
}
void SelectionMenuScene::create_deck_button(GameStructs::DeckType dt, const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto player = mngr->getHandler(ecs::hdlr::PLAYER);
    std::list<Card*> cl;
    buttonComp->connectClick([buttonComp, &mngr, player, dt, &cl]() {
        if (buttonComp->clicked()) return;
        buttonComp->set_clicked(true);
        
        switch (dt)
        {
        case GameStructs::ONE:
            cl = { new Recover(), new Fireball(), new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast()};
            break;
        case GameStructs::TWO: 
            cl = { new Recover(), new Fireball(), new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast() };
            break;
        case GameStructs::THREE:
            cl = { new Recover(), new Fireball(), new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast() };
            break;
        case GameStructs::FOUR:
            cl = { new Recover(), new Fireball(), new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast() };
            break;
        default:
            break;
        }
        mngr->addComponent<Deck>(player, cl);
        //Game::Instance()->change_Scene(Game::GAMESCENE);
        });

    buttonComp->connectHover([buttonComp]() {
        (void)buttonComp;
        });
}
void SelectionMenuScene::render() {
    //_selection->render(0,0); 
    Scene::render();
}