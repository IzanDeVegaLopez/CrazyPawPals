#include "RewardScene.h"

#include "GameStructs.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

#include "../our_scripts/card_system/Card.hpp"
#include "../our_scripts/card_system/CardList.h"
#include "../our_scripts/card_system/PlayableCards.hpp"
#include "../our_scripts/card_system/CardUpgrade.hpp"
#include "../our_scripts/components/Deck.hpp"
#include "../our_scripts/components/Button.h"
#include "../our_scripts/components/ImageForButton.h"

#include <iostream>
#include <string>


RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE), _mngr(Game::Instance()->get_mngr()), _reward_selected(false), _last_selected(nullptr),
_card(false), _health(false), _object(false), _upgrade(false)
{
}

RewardScene::~RewardScene()
{
}

void RewardScene::initScene() {
    float umbral = 0.4f;
    RandomNumberGenerator rand;

    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.5f, 0.2f}, {0.175f, 0.3f} },
        0.0f, ""
    };

    //Reward 1
    int card = rand.nextInt(0, 12);
    GameStructs::ButtonProperties reward1B = buttonPropTemplate;
    reward1B.sprite_key = select_cards(card);
    create_reward_button(reward1B);

    //std::cout << reward1B.rect.position.x << std::endl;

    //Reward 2
    card = rand.nextInt(0, 12);
    GameStructs::ButtonProperties reward2B = buttonPropTemplate;
    reward2B.sprite_key = select_cards(card);;
    reward2B.rect.position.x += umbral;
    create_reward_button(reward2B);

    //std::cout << reward2B.rect.position.x << std::endl;

    //Reward 3
    card = rand.nextInt(0, 12);
    GameStructs::ButtonProperties reward3B = buttonPropTemplate;
    reward3B.sprite_key = select_cards(card);;
    reward3B.rect.position.x -= umbral;
    create_reward_button(reward3B);

    //std::cout << reward3B.rect.position.x << std::endl;

    //Next Round
    GameStructs::ButtonProperties nextScene = buttonPropTemplate;
    nextScene.sprite_key = "play_button";
    nextScene.rect.position.y += 0.75f;
    nextScene.rect.position.x += 0.05f;
    nextScene.rect.size.x -= 0.1f;
    nextScene.rect.size.y -= 0.2f;
    create_next_round_button(nextScene);
   

}
void RewardScene::enterScene()
{
}

void RewardScene::exitScene()
{
}
void RewardScene::render() {

    Scene::render();
}

std::string RewardScene::select_cards(int index)
{
    //enum all_cards { FIREBALL, LIGHTNING, KUNAI, 
                    // RECOVER, MINIGUN, CARDSPRAY,
                    // ELDRICHBLAST, PRIMORDIA, COMMUNE,
                    // EVOKE, FULGUR, QUICKFEET }; //PRIME(? en revision
                    

    std::string sprite_key = "";

    switch (index)
    {
    case 0:
        sprite_key = "card_fireball";
        break;
    case 1:
        sprite_key = "card_lighting";
        break;
    case 2:
        sprite_key = "card_kunai";
        break;
    case 3:
        sprite_key = "card_recover";
        break;
    case 4:
        sprite_key = "card_minigun";
        break;
    case 5:
        sprite_key = "card_spray"; 
        break;
    case 6:
        sprite_key = "card_eldritch_blast";
        break;
    case 7:
        sprite_key = "card_primordia";
        break;
    case 8:
        sprite_key = "card_commune";
        break;
    case 9:
        sprite_key = "card_evoke";
        break;
    case 10:
        sprite_key = "card_fulgur";
        break;
    case 11:
        sprite_key = "card_quick_feet";
        break;
    default:
        break;
    }

    std::cout << index;

    return sprite_key;
   
}

void RewardScene::create_reward_button(const GameStructs::ButtonProperties& bp)
{
    auto e = create_button(bp);
    auto buttonComp = _mngr->getComponent<Button>(e);
    //Imagen del boton (seleccionado o sin seleccionar)
    /*auto imgComp = _mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at(bp.sprite_key + "_selected"),
        bp.rect,
        0,
        _mngr->getComponent<camera_component>(
        _mngr->getHandler(ecs::hdlr::CAMERA))->cam
    );*/

    buttonComp->connectClick([buttonComp,/* imgComp,*/ this]() {
        //std::cout << "left click -> Reward button" << std::endl;
        _reward_selected = true;

        //swap the actual buttons textures
        //imgComp->swap_textures();

        ////register the last clicked button
        //if (_last_selected != nullptr && _last_selected != imgComp) {
        //    _last_selected->swap_textures();
        //}
        //_last_selected = imgComp;

        //Add the selected card to the deck
        auto player = _mngr->getHandler(ecs::hdlr::PLAYER);
        auto m_deck = _mngr->getComponent<Deck>(player);

        auto cl = m_deck->DrawPile();
        //cl->add_card();
            
    });

    buttonComp->connectHover([buttonComp/*, imgComp*/]() {
        //std::cout << "hover -> Reward button" << std::endl;
        //filter
        //imgComp->apply_filter(128, 128, 128);
    });

    buttonComp->connectExit([buttonComp/*, imgComp*/]() {
        //std::cout << "exit -> Reward button" << std::endl;
        //filter
        //imgComp->apply_filter(255, 255, 255);
    });

}

void RewardScene::create_next_round_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    //Imagen del boton (seleccionado o sin seleccionar)
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at("p_boom"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );


    buttonComp->connectClick([buttonComp, &mngr]() {
        //std::cout << "left click -> next scene button" << std::endl;

        //Game::Instance()->change_Scene(Game::GAMESCENE);
    });


    buttonComp->connectHover([buttonComp, imgComp, this]() {

        //std::cout << "hover -> next scene button" << std::endl;
    });
}
