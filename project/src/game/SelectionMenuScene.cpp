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
#include <typeinfo>
#include <algorithm>
SelectionMenuScene::SelectionMenuScene() 
    : Scene(ecs::scene::SELECTIONMENUSCENE), _weapon_selected(false), _deck_selected(false), _last_weapon_button(nullptr), _last_deck_button(nullptr), 
    _num_cards_of_deck(6) {
}

SelectionMenuScene::~SelectionMenuScene()
{
}
void SelectionMenuScene::create_weapon_buttons() {
    float umbral = 0.25f;
    GameStructs::ButtonProperties buttonPropTemplate = {
         { {0.85f, 0.025f},{0.1f, 0.175f} },
         0.0f, "",  ecs::grp::WEAPONBUTTON
    };

    GameStructs::ButtonProperties revolverB = buttonPropTemplate;
    revolverB.sprite_key = "revolver_button";
    create_weapon_button(GameStructs::REVOLVER, revolverB);

    buttonPropTemplate.rect.position.y += umbral;
    GameStructs::ButtonProperties rampageB = buttonPropTemplate;
    rampageB.sprite_key = "rampage_button";
    create_weapon_button(GameStructs::RAMPAGE, rampageB);

    buttonPropTemplate.rect.position.y += umbral;
    GameStructs::ButtonProperties pump_shotgun_B = buttonPropTemplate;
    pump_shotgun_B.sprite_key = "pump_shotgun_button";
    create_weapon_button(GameStructs::PUMP_SHOTGUN, pump_shotgun_B);

    buttonPropTemplate.rect.position.y += umbral;
    GameStructs::ButtonProperties ramp_canon_B = buttonPropTemplate;
    ramp_canon_B.sprite_key = "ramp_canon_button";
    create_weapon_button(GameStructs::RAMP_CANON, ramp_canon_B);

    buttonPropTemplate.rect.position.y += umbral;
    GameStructs::ButtonProperties lightbringerB = buttonPropTemplate;
    lightbringerB.sprite_key = "lightbringer_button";
    create_weapon_button(GameStructs::LIGHTBRINGER, lightbringerB);
}

void SelectionMenuScene::create_deck_buttons() {
    float size = 0.3f;
    float umbral = 0.2f;
    //create the first button prop
    GameStructs::ButtonProperties buttonPropTemplate = {
         { {0.025f, 0.025f},{0.2f, 0.3f} },
         0.0f, "", ecs::grp::DECKBUTTON
    };
    GameStructs::ButtonProperties deck1B = buttonPropTemplate;
    deck1B.sprite_key = "deck1_button";
    create_deck_button(GameStructs::ONE, deck1B);

    buttonPropTemplate.rect.position.x += umbral;
    GameStructs::ButtonProperties deck2B = buttonPropTemplate;
    deck2B.sprite_key = "deck2_button";
    create_deck_button(GameStructs::TWO, deck2B);

    buttonPropTemplate.rect.position.x += umbral;
    GameStructs::ButtonProperties deck3B = buttonPropTemplate;
    deck3B.sprite_key = "deck3_button";
    create_deck_button(GameStructs::THREE, deck3B);

    buttonPropTemplate.rect.position.x += umbral;
    GameStructs::ButtonProperties deck4B = buttonPropTemplate;
    deck4B.sprite_key = "deck4_button";
    create_deck_button(GameStructs::FOUR, deck4B);
}
void SelectionMenuScene::initScene() {
    _selection = &sdlutils().images().at("selection");
    create_weapon_info();
    create_weapon_buttons();
    create_deck_buttons();
    create_deck_infos();
    create_enter_button();
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
        &sdlutils().images().at(bp.sprite_key + "_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );
    auto player = mngr->getHandler(ecs::hdlr::PLAYER);

    buttonComp->connectClick([buttonComp, imgComp, mngr, wt, player, this]() {
        //std::cout << "left click-> button" << std::endl;
        std::string s;

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

        if (imgComp != _last_weapon_button) {
            //swap the actual buttons textures
            imgComp->swap_textures();
        }

        //register the last clicked button
        if (_last_weapon_button != nullptr && _last_weapon_button != imgComp) {
            _last_weapon_button->swap_textures();
        }
        _last_weapon_button = imgComp;

        

    });
    buttonComp->connectHover([buttonComp, imgComp, mngr, wt]() {
        std::string s;
        auto& info = mngr->getEntities(ecs::grp::WEAPONINFO);
        assert(!info.empty());
        auto infoImg = mngr->getComponent<transformless_dyn_image>(info[0]);
        switch (wt) {
        case GameStructs::REVOLVER:
            s = "revolver_";
            break;
        case GameStructs::RAMPAGE:
            s = "rampage_";
            break;
        case GameStructs::PUMP_SHOTGUN:
            s = "pump_shotgun_";
            break;
        case GameStructs::RAMP_CANON:
            s = "ramp_canon_";
            break;
        case GameStructs::LIGHTBRINGER:
            s = "lightbringer_";
            break;
        default:
            break;
        }
        s += "info";
        infoImg->set_texture(&sdlutils().images().at(s));
        imgComp->apply_filter(128,128,128);
    });
    buttonComp->connectExit([buttonComp, imgComp,mngr]() {
        auto& info = mngr->getEntities(ecs::grp::WEAPONINFO);
        assert(!info.empty());
        auto infoImg = mngr->getComponent<transformless_dyn_image>(info[0]);
        infoImg->set_texture(&sdlutils().images().at("initial_info"));
        imgComp->apply_filter(255, 255, 255);
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
        &sdlutils().images().at(bp.sprite_key + "_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );
    buttonComp->connectClick([buttonComp, imgComp, mngr, player, dt, this]() {
        std::list<Card*> cl = {};
        
        switch (dt)
        {
        case GameStructs::ONE:
            cl = { new Prime(), new Primordia(), new Prime(), new Prime(), new Prime(), new Prime() };
            break;
        case GameStructs::TWO: 
            cl = { new Fireball(), new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast() };
            break;
        case GameStructs::THREE:
            cl = { new CardSpray(), new Lighting(), new Minigun(), new Kunai(), new EldritchBlast(), new Recover() };
            break;
        case GameStructs::FOUR:
            cl = { new Lighting(), new Minigun(), new Kunai(), new EldritchBlast(), new Commune(), new QuickFeet()};
            break;
        default:
            break;
        }

        set_concrete_deck_info(cl);

        mngr->addComponent<Deck>(player, cl);
        _deck_selected = true;

        //swap the actual buttons textures
        if (_last_deck_button != nullptr &&_last_deck_button != imgComp) {
            imgComp->swap_textures();
            _last_deck_button->swap_textures();
            //register the clicked button
        }
        else if (_last_deck_button == nullptr) { //special case: first click
            imgComp->swap_textures();
        }
        _last_deck_button = imgComp;
    });

    buttonComp->connectHover([buttonComp, imgComp]() {
        imgComp->apply_filter(128, 128, 128);
        });
    buttonComp->connectExit([buttonComp, imgComp]() {
        imgComp->apply_filter(255, 255, 255);
        });
}
void SelectionMenuScene::create_deck_info(const rect_f32& rect) {
    ecs::entity_t e = create_entity(
        ecs::grp::DECKINFO,
        ecs::scene::SELECTIONMENUSCENE,
        new transformless_dyn_image
        (rect, 
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam,
        &sdlutils().images().at("initial_info"))
    );
}

void SelectionMenuScene::create_deck_infos() {
    rect_f32 r = {{ 0.05f, 0.325f }, { 0.55f, 0.25f }};
    for (int i = 0; i < _num_cards_of_deck; ++i) {
        create_deck_info(r); 
        r.position.y += 0.25f;
    }
}
void SelectionMenuScene::create_weapon_info() {
   // rect_f32 rect = {{1.3f, 0.25f} ,{0.75f, 0.5f}};
    rect_f32 rect = { {1.0f, 0.25f} ,{0.75f, 0.5f} };
    ecs::entity_t e = create_entity(
        ecs::grp::WEAPONINFO,
        ecs::scene::SELECTIONMENUSCENE,
        new transformless_dyn_image
        (rect,
            0,
            Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam,
            &sdlutils().images().at("initial_info"))
    );
    auto i = Game::Instance()->get_mngr();
    int ew = 0;

}
void SelectionMenuScene::render() {
    _selection->render(0, -60);
    Scene::render();
}

void SelectionMenuScene::set_concrete_deck_info(const std::list<Card*>& cl) {
    auto mngr = Game::Instance()->get_mngr();
    auto infos = mngr->getEntities(ecs::grp::DECKINFO);

    auto itInfo = infos.begin();
    for (auto it = cl.begin(); it != cl.end() && itInfo != infos.end(); ++it, ++itInfo) {
        auto img = mngr->getComponent<transformless_dyn_image>(*itInfo);
        std::string typeName = typeid(**it).name();
        std::string prefix = "class ";
        if (typeName.find(prefix) == 0) {  // Si empieza con "class "
            typeName = typeName.substr(prefix.size());  // Elimina "class "
        }
        img->set_texture(&sdlutils().images().at("card_" + typeName + "_info"));
    }
}
void SelectionMenuScene::create_enter_button() {
    GameStructs::ButtonProperties bp = {
         { {0.5f, 0.5f},{0.3f, 0.125f} },
         0.0f, "enter_game", ecs::grp::UI
    };
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);

    buttonComp->connectClick([buttonComp, mngr, this]() {
        if (_weapon_selected &&_deck_selected)
        Game::Instance()->change_Scene(Game::REWARDSCENE);
    }); 
}
