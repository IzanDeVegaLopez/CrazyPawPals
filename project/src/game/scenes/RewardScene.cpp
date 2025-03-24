#include "RewardScene.h"
#include "../../our_scripts/components/ui/Button.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"

#include "../../our_scripts/card_system/Card.hpp"
#include "../../our_scripts/card_system/CardList.h" 
#include "../../our_scripts/card_system/PlayableCards.hpp"
#include "../../our_scripts/components/cards/Deck.hpp"
#include "../../our_scripts/components/rendering/ImageForButton.h"

#include <iostream>

RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE),_selected_card(nullptr), _selected_button(nullptr), _reward_bg(nullptr)
{
}

RewardScene::~RewardScene()
{
}

void RewardScene::initScene() {
    create_reward_buttons();
    create_my_deck_cards();
    _reward_bg = &sdlutils().images().at("reward");
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
    _reward_bg->render(0, 0);
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
ecs::entity_t RewardScene::create_card_button(const GameStructs::CardButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto b = mngr->addComponent<CardButton>(e);
    b->initComponent();
    b->set_it(bp.iterator);
    return e;
}
void RewardScene::create_a_deck_card(const GameStructs::CardButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();

    auto e = create_card_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    buttonComp->connectClick([buttonComp, imgComp, this, bp] {
        imgComp->destination_rect.size = { imgComp->_original_w,  imgComp->_original_h };
        _selected_button = buttonComp;
        imgComp->destination_rect.position.y = bp.rect.position.y;
        //only assign a valid iterator
        if (bp.iterator != nullptr && bp.iterator != _selected_card) {
            _selected_card = bp.iterator;
            std::cout << "card selected "<< std::endl;
        }
        });

    buttonComp->connectHover([buttonComp, imgComp]() {
        std::cout << "hover -> Reward button: " << std::endl;
        //filter
        //imgComp->apply_filter(128, 128, 128);
        imgComp->destination_rect.position.y -= 0.125f;
        //imgComp->destination_rect.size = { imgComp->destination_rect.size.x * 1.25f,  imgComp->destination_rect.size.y * 1.25f };
        });

    buttonComp->connectExit([buttonComp, imgComp]() {
        std::cout << "exit -> Reward button: " << std::endl;
        imgComp->destination_rect.position.y += 0.125f;
        //filter
        //imgComp->destination_rect.size = { imgComp->destination_rect.size.x / 1.25f,  imgComp->destination_rect.size.y / 1.25f };
        });
}
void RewardScene::create_my_deck_cards() {
    auto* mngr = Game::Instance()->get_mngr();

    //GET PLAYERS DECK REFERENCE
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    if (player && !mngr->hasComponent<Deck>(player)) {
        //when we add these entities, our olayer doesnt have any deck as component
        mngr->addComponent<Deck>(player);
    }
    auto _m_deck = mngr->getComponent<Deck>(player);
    auto& pDeck = _m_deck->card_names();

    float umbral = 0.15f;
    auto iterator = _m_deck->all_cards().card_list().begin();
    GameStructs::CardButtonProperties propTemplate = {
        { {0.1f, 0.95f}, {0.125f, 0.25f} },
        0.0f, "", ecs::grp::REWARDCARDS, *iterator
    };

    for (const auto& it : pDeck) {
        propTemplate.sprite_key = it;
        create_a_deck_card(propTemplate);
        propTemplate.rect.position.x += umbral;
        iterator++;
        if (iterator == _m_deck->all_cards().card_list().end()) {
            propTemplate.iterator = nullptr;
        }
    }
    for (int i = 0; i < 4; ++i) {
        propTemplate.sprite_key = "initial_info";
        create_a_deck_card(propTemplate);
        propTemplate.rect.position.x += umbral;
    }
}
void RewardScene::refresh_my_deck_cards(const std::list<std::string>& cl) {
    auto* mngr = Game::Instance()->get_mngr();
    auto infos = mngr->getEntities(ecs::grp::REWARDCARDS);

    // Obtener la referencia al mazo actualizado
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto* deck = mngr->getComponent<Deck>(player);
    auto& updatedCardList = deck->all_cards().card_list(); // Obtener los nuevos punteros

    auto itInfo = infos.begin();
    auto itCard = updatedCardList.begin();
    auto it = cl.begin();

    // Refrescar las texturas y actualizar punteros
    for (; it != cl.end() && itCard != updatedCardList.end(); ++it, ++itInfo, ++itCard) {
        auto img = mngr->getComponent<transformless_dyn_image>(*itInfo);
        img->set_texture(&sdlutils().images().at(*it));

        // Actualizar la referencia a la carta en el botón
        auto buttonComp = mngr->getComponent<Button>(*itInfo);
        if (buttonComp) {
            static_cast<CardButton*>(buttonComp)->set_it(*itCard); // Método para actualizar puntero
        }
    }

    // Rellenar con imágenes vacías si hay menos cartas en el nuevo mazo
    for (; itInfo != infos.end(); ++itInfo) {
        auto img = mngr->getComponent<transformless_dyn_image>(*itInfo);
        img->set_texture(&sdlutils().images().at("initial_info"));

        auto buttonComp = mngr->getComponent<Button>(*itInfo);
        if (buttonComp) {
            static_cast<CardButton*>(buttonComp)->set_it(nullptr); // Poner puntero a nullptr si no hay carta
        }
    }
}