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
#include <unordered_set>

RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE),_selected_card(nullptr), _selected_button(nullptr), _reward_bg(nullptr),
_health(false),_card(false), _object(false), _upgrade(false), _lr(nullptr)
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
std::string RewardScene::select_card(GameStructs::CardType ct) {
    std::string s = "";
    switch (ct)
    {
    case GameStructs::FIREBALL: s = "reward_card_fireball";
        break;
    case GameStructs::LIGHTING: s = "reward_card_lighting";
        break;
    case GameStructs::KUNAI: s = "reward_card_kunai";
        break;
    case GameStructs::RECOVER: s = "reward_card_recover";
        break;
    case GameStructs::MINIGUN: s = "reward_card_minigun";
        break;
    case GameStructs::SPRAY: s = "reward_card_spray";
        break;
    case GameStructs::ELDRITCH_BLAST: s = "reward_card_eldritch_blast";
        break;
    case GameStructs::COMMUNE: s = "reward_card_commune";
        break;
    case GameStructs::EVOKE: s = "reward_card_evoke";
        break;
    case GameStructs::FULGUR: s = "reward_card_fulgur";
        break;
    case GameStructs::QUICK_FEET: s = "reward_card_quick_feet";
        break;
    default:
        break;
    }
    std::cout << "reward card: " + s << std::endl;
    return s;
}
void RewardScene::create_reward_buttons() {
    float umbral = 0.4f;
    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.5f, 0.2f}, {0.175f, 0.3f} },
        0.0f, ""
    };

    // Auxiliar set para asegurarnos de que no se repiten cartas
    std::unordered_set<std::string> appeared_cards;

    //lambda function
    auto get_unique_card = [&](GameStructs::CardType& ct) -> std::string { 
        std::string sprite; 
        do {
            ct = (GameStructs::CardType)sdlutils().rand().nextInt(0, GameStructs::LAST_CARD); 
            sprite = select_card(ct); 
        } while (appeared_cards.find(sprite) != appeared_cards.end()); // Repeat this process if we got some card that appeared before 
        appeared_cards.insert(sprite); 
        return sprite; 
        };

    //three card reward buttons
    GameStructs::ButtonProperties reward_card1 = buttonPropTemplate; 
    GameStructs::CardType ct; 
    reward_card1.sprite_key = get_unique_card(ct); 
    create_reward_card_button(reward_card1);

    GameStructs::ButtonProperties reward_card2 = buttonPropTemplate; 
    reward_card2.sprite_key = get_unique_card(ct); 
    reward_card2.rect.position.x -= umbral; 
    create_reward_card_button(reward_card2);
     
    GameStructs::ButtonProperties reward_card3 = buttonPropTemplate; 
    reward_card3.sprite_key = get_unique_card(ct); 
    reward_card3.rect.position.x += umbral; 
    create_reward_card_button(reward_card3);

    //it only appears in certain circustances (if so, we swap the position between this button and the third reward card)
    GameStructs::ButtonProperties reward_heal = buttonPropTemplate;
    reward_heal.sprite_key = "reward_health";
    reward_heal.rect.position.x = 20.0f;
    create_reward_health_button(reward_heal);
}
void RewardScene::create_reward_health_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    //used for change the sprite once a button is clicked
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at("reward_health_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );

    buttonComp->connectClick([buttonComp, imgComp]() {
        std::cout << "left click -> Reward button" << std::endl;
        //swap the actual buttons textures
        imgComp->swap_textures();

        ////swap the actual buttons textures
        //if (_last_reward_button != nullptr && _last_reward_button != imgComp) {
        //    imgComp->swap_textures();
        //    _last_deck_button->swap_textures();
        //    //register the clicked button
        //}
        //else if (_last_deck_button == nullptr) { //special case: first click
        //    imgComp->swap_textures();
        //}
        //_last_deck_button = imgComp;
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
void RewardScene::create_reward_card_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    
    //used for change the sprite once a button is clicked
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key), 
        &sdlutils().images().at("reward_card_selected"),
        bp.rect, 
        0, 
        Game::Instance()->get_mngr()->getComponent<camera_component>( 
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam 
    ); 
     
    buttonComp->connectClick([buttonComp, imgComp]() {
        std::cout << "left click -> Reward button" << std::endl;
        //swap the actual buttons textures
        imgComp->swap_textures();

        ////swap the actual buttons textures
        //if (_last_reward_button != nullptr && _last_reward_button != imgComp) {
        //    imgComp->swap_textures();
        //    _last_deck_button->swap_textures();
        //    //register the clicked button
        //}
        //else if (_last_deck_button == nullptr) { //special case: first click
        //    imgComp->swap_textures();
        //}
        //_last_deck_button = imgComp;
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