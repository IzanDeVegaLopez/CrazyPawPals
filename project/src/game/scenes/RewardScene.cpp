#include "RewardScene.h"
#include "../../our_scripts/components/ui/Button.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"

#include "../../our_scripts/card_system/Card.hpp"
#include "../../our_scripts/card_system/PlayableCards.hpp"
#include "../../our_scripts/components/cards/Deck.hpp"
#include "../../our_scripts/components/rendering/ImageForButton.h"

#include <iostream>

RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE),_selected_card(nullptr)
{
}

RewardScene::~RewardScene()
{
}

void RewardScene::initScene() {
    create_reward_buttons();
    create_my_deck_cards();
}
void RewardScene::enterScene()
{
    auto* mngr = Game::Instance()->get_mngr();
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto _m_deck = mngr->getComponent<Deck>(player);
    auto& pDeck = _m_deck->card_names();
    refresh_my_deck_cards(pDeck);
}

void RewardScene::exitScene()
{
}
void RewardScene::render() {

    Scene::render();
}
void RewardScene::create_reward_buttons() {
    float umbral = 0.4f;
    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.5f, 0.2f}, {0.175f, 0.3f} },
        0.0f, ""
    };

    GameStructs::ButtonProperties reward1B = buttonPropTemplate;
    reward1B.sprite_key = "reward_card";
    create_reward_button(reward1B);

    GameStructs::ButtonProperties reward2B = buttonPropTemplate;
    reward2B.sprite_key = "reward_card";
    reward2B.rect.position.x += umbral;
    create_reward_button(reward2B);

    GameStructs::ButtonProperties reward3B = buttonPropTemplate;
    reward3B.sprite_key = "reward_card";
    reward3B.rect.position.x -= umbral;
    create_reward_button(reward3B);
}
void RewardScene::create_reward_button(const GameStructs::ButtonProperties& bp)
{
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
     
    buttonComp->connectClick([buttonComp, imgComp]() {
        std::cout << "left click -> Reward button" << std::endl;
        //swap the actual buttons textures
        imgComp->swap_textures();
        });

    buttonComp->connectHover([buttonComp, imgComp]() {
        std::cout << "hover -> Reward button: " << std::endl;
        //filter
        imgComp->apply_filter(128, 128, 128);
        });

    buttonComp->connectExit([buttonComp, imgComp]() {
        std::cout << "exit -> Reward button: " << std::endl;
        //filter
        imgComp->apply_filter(255, 255, 255);
        });

}
void RewardScene::create_a_deck_card(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();

    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    buttonComp->connectClick([buttonComp, imgComp, this] {
        imgComp->destination_rect.size = { imgComp->_original_w,  imgComp->_original_h };
        _selected_card = buttonComp;
    });

    buttonComp->connectHover([buttonComp, imgComp]() {
        std::cout << "hover -> Reward button: " << std::endl;
        //filter
        //imgComp->apply_filter(128, 128, 128);
        imgComp->destination_rect.size = { imgComp->destination_rect.size.x*1.25f,  imgComp->destination_rect.size.y * 1.25f };
    });

    buttonComp->connectExit([buttonComp, imgComp]() {
        std::cout << "exit -> Reward button: " << std::endl;
        //filter
        //imgComp->apply_filter(255, 255, 255);
        imgComp->destination_rect.size = { imgComp->destination_rect.size.x / 1.25f,  imgComp->destination_rect.size.y / 1.25f };
    });
}
void RewardScene::create_my_deck_cards() {
    float umbral = 0.175f;
    GameStructs::ButtonProperties propTemplate = {
        { {0.15f, 0.7f}, {0.175f, 0.3f} },
        0.0f, "", ecs::grp::REWARDCARDS
    };
    auto* mngr = Game::Instance()->get_mngr();

    //GET PLAYERS DECK REFERENCE
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    if (player && !mngr->hasComponent<Deck>(player)) {
        //when we add these entities, our olayer doesnt have any deck as component
       mngr->addComponent<Deck>(player);
    }
   auto _m_deck = mngr->getComponent<Deck>(player);
   auto& pDeck = _m_deck->card_names();
    
   for (const auto& it : pDeck) {
        propTemplate.sprite_key = it;
        create_a_deck_card(propTemplate);
        propTemplate.rect.position.x += umbral;
   }
   for (int i = 0; i < 4; ++i) {
       propTemplate.sprite_key = "initial_info";
       create_a_deck_card(propTemplate);
       propTemplate.rect.position.x += umbral;
   }
}
void RewardScene::refresh_my_deck_cards(const std::list<std::string>& cl) {
  
    auto mngr = Game::Instance()->get_mngr();
    auto infos = mngr->getEntities(ecs::grp::REWARDCARDS);

    auto itInfo = infos.begin();
    auto it = cl.begin();
    for (; it != cl.end(); ++it, ++itInfo) {
        auto img = mngr->getComponent<transformless_dyn_image>(*itInfo);
        img->set_texture(&sdlutils().images().at(*it));
    }
    for (; itInfo != infos.end(); ++itInfo) {
        auto img = mngr->getComponent<transformless_dyn_image>(*itInfo);
        img->set_texture(&sdlutils().images().at("initial_info"));
    }
}