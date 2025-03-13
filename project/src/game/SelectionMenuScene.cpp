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
#include "../our_scripts/components/Image.h"
#include "../our_scripts/components/ImageForButton.h"
#include <iostream>
#include <string>
#include <list>


SelectionMenuScene::SelectionMenuScene() 
    : Scene(ecs::scene::SELECTIONMENUSCENE), _weapon_selected(false), _deck_selected(false), _last_weapon_button(nullptr), _last_deck_button(nullptr) {

}

SelectionMenuScene::~SelectionMenuScene()
{
}
void SelectionMenuScene::create_weapon_buttons() {
    int i = 0;
    float thrust = 0.175f;
    float umbral = 50.0f;
    float h = sdlutils().height() * thrust * i++ + umbral;
    GameStructs::ButtonProperties buttonPropTemplate = { {sdlutils().width() * 1.4f, h},
        100.0f, 100.0f, 0.0f, "", ecs::grp::WEAPONBUTTON
    };
    GameStructs::ButtonProperties revolverB = buttonPropTemplate;
    revolverB.sprite_key = "revolver_button";
    create_weapon_button(GameStructs::REVOLVER, revolverB);

    GameStructs::ButtonProperties rampageB = buttonPropTemplate;
    rampageB.sprite_key = "rampage_button";
    h = sdlutils().height() * thrust * i++ + umbral;
    rampageB.pos.setY(h);
    create_weapon_button(GameStructs::RAMPAGE, rampageB);

    GameStructs::ButtonProperties pump_shotgun_B = buttonPropTemplate;
    pump_shotgun_B.sprite_key = "pump_shotgun_button";
    h = sdlutils().height() * thrust * i++ + umbral;
    pump_shotgun_B.pos.setY(h);
    create_weapon_button(GameStructs::PUMP_SHOTGUN, pump_shotgun_B);

    GameStructs::ButtonProperties ramp_canon_B = buttonPropTemplate;
    ramp_canon_B.sprite_key = "ramp_canon_button";
    h = sdlutils().height() * thrust * i++ + umbral;
    ramp_canon_B.pos.setY(h);
    create_weapon_button(GameStructs::RAMP_CANON, ramp_canon_B);

    GameStructs::ButtonProperties lightbringerB = buttonPropTemplate;
    lightbringerB.sprite_key = "lightbringer_button";
    h = sdlutils().height() * thrust * i++ + umbral;
    lightbringerB.pos.setY(h);
    create_weapon_button(GameStructs::LIGHTBRINGER, lightbringerB);
}

void SelectionMenuScene::create_deck_buttons() {
    int i = 0;
    float thrust = 0.15f;
    float umbral = 30.0f;
    float w = sdlutils().width() * thrust * i++ + umbral;
    GameStructs::ButtonProperties buttonPropTemplate = { {w, 50.0f},
        125.0f, 125.0f, 0.0f, "", ecs::grp::DECKBUTTON
    };
    GameStructs::ButtonProperties deck1B = buttonPropTemplate;
    deck1B.sprite_key = "deck1_button";
    create_deck_button(GameStructs::ONE, deck1B);

    GameStructs::ButtonProperties deck2B = buttonPropTemplate;
    deck2B.sprite_key = "deck2_button";
    w = sdlutils().width() * thrust * i++ + umbral;
    deck2B.pos.setX(w);
    create_deck_button(GameStructs::TWO, deck2B);

    GameStructs::ButtonProperties deck3B = buttonPropTemplate;
    deck3B.sprite_key = "deck3_button";
    w = sdlutils().width() * thrust * i++ + umbral;
    deck3B.pos.setX(w);
    create_deck_button(GameStructs::THREE, deck3B);

    GameStructs::ButtonProperties deck4B = buttonPropTemplate;
    deck4B.sprite_key = "deck4_button";
    w = sdlutils().width() * thrust * i++ + umbral;
    deck4B.pos.setX(w);
    create_deck_button(GameStructs::FOUR, deck4B);
}
void SelectionMenuScene::initScene() {
    _selection = &sdlutils().images().at("selection");
    create_weapon_buttons();
    create_deck_buttons();
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
    //used for change the sprite once a button is clicked
    auto imgComp = mngr->addComponent<ImageForButton>(e, 
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at(bp.sprite_key + "_selected"));
    auto player = mngr->getHandler(ecs::hdlr::PLAYER);
    buttonComp->connectClick([buttonComp, imgComp, &mngr, wt, player, this]() {
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
        _weapon_selected = true;

        //swap the actual buttons textures
        imgComp->swap_textures();

        //register the last clicked button
        if (_last_weapon_button != nullptr && _last_weapon_button != imgComp) {
            _last_weapon_button->swap_textures();
        }
        _last_weapon_button = imgComp;

        //condition to start the game
        if (_deck_selected) Game::Instance()->change_Scene(Game::GAMESCENE);
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
    //used for change the sprite once a button is clicked
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at(bp.sprite_key + "_selected"));

    buttonComp->connectClick([buttonComp, imgComp, &mngr, player, dt, this]() {
        std::list<Card*> cl = {};
        
        switch (dt)
        {
        case GameStructs::ONE:
            cl.push_back(new Recover());
            break;
        case GameStructs::TWO: 
            cl = { new Fireball(), new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast() };
            break;
        case GameStructs::THREE:
            cl = { new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast() };
            break;
        case GameStructs::FOUR:
            cl = { new Lighting(), new Minigun(), new Kunai(), new EldritchBlast() };
            break;
        default:
            break;
        }
        mngr->addComponent<Deck>(player, cl);
        _deck_selected = true;

        //swap the actual buttons textures
        imgComp->swap_textures();

        //register the last clicked button
        if (_last_deck_button != nullptr && _last_deck_button != imgComp) {
            _last_deck_button->swap_textures();
        }
        _last_deck_button = imgComp;
        //condition to start the game
        if (_weapon_selected) Game::Instance()->change_Scene(Game::GAMESCENE);
    });

    buttonComp->connectHover([buttonComp]() {
    }
);
}
void SelectionMenuScene::render() {
    //_selection->render(0,0); 
    Scene::render();
}