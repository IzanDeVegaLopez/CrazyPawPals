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
#include "../../our_scripts/components/cards/RewardDataComponent.h"
#include "../../our_scripts/components/Health.h" 

#include <iostream>

RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE),_selected_card(nullptr), _selected_button(nullptr), _reward_bg(nullptr),
_health(false),_card(false), _object(false), _upgrade(false), _lr(nullptr), 
_selected(false), _activate_confirm_button(false), _activate_exchange_button(false)
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
    refresh_rewards();
}

void RewardScene::exitScene()
{
    change_pos(false);
}
void RewardScene::render() {
    _reward_bg->render(0, 0);
    Scene::render();
}
std::string RewardScene::get_unique_card(GameStructs::CardType& ct, std::unordered_set<std::string>& appeared_cards) {
    std::string sprite;
    do {
        ct = (GameStructs::CardType)sdlutils().rand().nextInt(0, GameStructs::LAST_CARD);
        sprite = select_card(ct);
    } while (appeared_cards.find(sprite) != appeared_cards.end()); // Repite si la carta ya apareció

    appeared_cards.insert(sprite);
    return sprite;
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

void RewardScene::refresh_rewards() {
    auto* mngr = Game::Instance()->get_mngr();
    auto& rewards_cards = mngr->getEntities(ecs::grp::REWARDCARDS);
    std::unordered_set<std::string> appeared_cards;
    GameStructs::CardType ct;
    //refresh the three reward card button
    for (auto& e : rewards_cards) {
        auto s = get_unique_card(ct, appeared_cards);
        auto img = mngr->getComponent<transformless_dyn_image>(e);
        img->set_texture(&sdlutils().images().at(s));
        auto data = mngr->getComponent<RewardDataComponent>(e);
        data->_data = s;
    }

    change_pos(true);
}

void RewardScene::change_pos(bool enter) {
    auto swap_positions = [](transformless_dyn_image* img1, transformless_dyn_image* img2) {
        auto aux = img1->destination_rect.position.x;
        img1->destination_rect.position.x = img2->destination_rect.position.x;
        img2->destination_rect.position.x = aux;
        };

    auto* mngr = Game::Instance()->get_mngr();
    auto& rewardCard = mngr->getEntities(ecs::grp::REWARDCARDS)[2];

    // players health
    auto playerHealth = mngr->getComponent<Health>(mngr->getHandler(ecs::hdlr::PLAYER));
    int act = playerHealth->getHealth();
    int max = playerHealth->getMaxHealth();

    // other references
    auto healthReward = mngr->getHandler(ecs::hdlr::REWARDHEALTH);
    auto img = mngr->getComponent<transformless_dyn_image>(rewardCard); // Última carta
    auto healImg = mngr->getComponent<transformless_dyn_image>(healthReward);

    auto bRewardButton = mngr->getComponent<Button>(rewardCard);
    auto hRewardButton = mngr->getComponent<Button>(healthReward);

    if (enter) { //if we need to activate the heal reward
        if ((float)act / (float)max <= 0.2f) {
            swap_positions(img, healImg);
            bRewardButton->update_collider();
            hRewardButton->update_collider();
        }
    }
    else { //in other case, the condition to swap changes
        if ((float)act / (float)max > 0.2f) {
            swap_positions(img, healImg);
            bRewardButton->update_collider();
            hRewardButton->update_collider();
        }
    }
}

void RewardScene::create_reward_buttons() {
    float umbral = 0.4f;
    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.5f, 0.1f}, {0.175f, 0.3f} },
        0.0f, "", ecs::grp::REWARDCARDS
    };

    // Auxiliar set to check if we repeat some card
    std::unordered_set<std::string> appeared_cards;

    //three card reward buttons
    GameStructs::CardType ct; 
    buttonPropTemplate.sprite_key = get_unique_card(ct, appeared_cards);
    create_reward_card_button(buttonPropTemplate);

    buttonPropTemplate.sprite_key = get_unique_card(ct, appeared_cards);
    buttonPropTemplate.rect.position.x -= umbral;
    create_reward_card_button(buttonPropTemplate);
     
    buttonPropTemplate.sprite_key = get_unique_card(ct, appeared_cards);
    buttonPropTemplate.rect.position.x += umbral*2;
    create_reward_card_button(buttonPropTemplate);

    //it only appears in certain circustances (if so, we swap the position between this button and the third reward card)
    buttonPropTemplate.ID = ecs::grp::UI;
    buttonPropTemplate.sprite_key = "reward_health";
    buttonPropTemplate.rect.position.x = 20.0f;
    create_reward_health_button(buttonPropTemplate);

    //selected button
    buttonPropTemplate.ID = ecs::grp::UI;
    buttonPropTemplate.sprite_key = "confirm_reward";
    buttonPropTemplate.rect.position = { 0.5f, 0.5f };
    buttonPropTemplate.rect.size = { 0.325f, 0.195f };
    create_reward_selected_button(buttonPropTemplate);

    //exchange button
    buttonPropTemplate.ID = ecs::grp::UI;
    buttonPropTemplate.sprite_key = "confirm_reward";
    buttonPropTemplate.rect.position = { 0.25f, 0.5f };
    buttonPropTemplate.rect.size = { 0.325f, 0.195f };
    create_reward_selected_button(buttonPropTemplate);
}

void RewardScene::create_reward_health_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    mngr->setHandler(ecs::hdlr::REWARDHEALTH, e);

    auto buttonComp = mngr->getComponent<Button>(e);
    //used for change the sprite once a button is clicked
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at("reward_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );
    auto data = mngr->addComponent<RewardDataComponent>(e, bp.sprite_key);

    buttonComp->connectClick([buttonComp, imgComp, this]() {
        if (_selected) {
            std::cout << "already selected" << std::endl;
        }
        else {
            std::cout << "left click -> health button" << std::endl;
            if (_lr == nullptr) {
                resize(imgComp, 1.1f);
            }
            else if (_lr != nullptr && _lr != imgComp) {
                resize(imgComp, 1.1f);
                resize(_lr, 1.0f / 1.1f);
            }
            _lr = imgComp;
        }
    });
    buttonComp->connectHover([buttonComp, imgComp, this]() {
        if (_selected) return;
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

void RewardScene::resize(ImageForButton* im, float factor) { im->destination_rect.size = { 
    im->destination_rect.size.x * factor, im->destination_rect.size.y * factor };
}

void RewardScene::create_reward_card_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    
    //used for change the sprite once a button is clicked
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key), 
        &sdlutils().images().at("reward_selected"),
        bp.rect, 
        0, 
        Game::Instance()->get_mngr()->getComponent<camera_component>( 
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam 
    );  
    //used for change the sprite once a button is clicked
    auto data = mngr->addComponent<RewardDataComponent>(e, bp.sprite_key);

    buttonComp->connectClick([buttonComp, imgComp, this]() {
        if (_selected) {
            std::cout << "already selected" << std::endl;
        }
        else {
            std::cout << "left click -> reward card button" << std::endl;
            if (_lr == nullptr) {
                resize(imgComp, 1.1f);
            }
            else if (_lr != nullptr && _lr != imgComp) {
                resize(imgComp, 1.1f);
                resize(_lr, 1.0f / 1.1f);
            }
            _lr = imgComp;
        }
    });
    buttonComp->connectHover([buttonComp, imgComp, this]() {
        if (_selected) return;
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
        0.0f, "", ecs::grp::REWARDDECK, *iterator
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
    auto infos = mngr->getEntities(ecs::grp::REWARDDECK);

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

void RewardScene::create_reward_selected_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();

    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at("initial_info"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );
    imgComp->swap_textures();
    mngr->setHandler(ecs::hdlr::CONFIRMREWARD,e);
    buttonComp->connectClick([buttonComp, this, imgComp] {
        //we only select a reward if previously we have chosen something
        if (_lr != nullptr && !_selected) {
            _lr->apply_filter(255, 255, 255);
            _lr->swap_textures();
            _selected = true;

            //We get the reference to the deck
            auto* mngr = Game::Instance()->get_mngr();
            auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
            auto _m_deck = mngr->getComponent<Deck>(player);

            //We add the cards to the deck
            _m_deck->add_card_to_deck(new Fireball());
            
            //Refresh deck
            auto& pDeck = _m_deck->card_names();
 
            refresh_my_deck_cards(pDeck);

            imgComp->swap_textures();
        }
    });
    buttonComp->connectHover([buttonComp, imgComp, this]() {
        if (_selected) return;
        std::cout << "hover -> Reward selected button: " << std::endl;
        //filter
        imgComp->apply_filter(128, 128, 128);
    });
    buttonComp->connectExit([buttonComp, imgComp, this]() {
        std::cout << "exit -> Reward selected button: " << std::endl;
        //filter
        imgComp->apply_filter(255, 255, 255);
    });
}
void RewardScene::exchange_deck_card()
{
    //We get the reference to the deck
    auto* mngr = Game::Instance()->get_mngr();
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto _m_deck = mngr->getComponent<Deck>(player);

    //Removes old card from deck
    //Look m
   // _m_deck->all_cards().erase_card(_selected_card);


}

void RewardScene::check_number()
{
    //Once reward is picked, checks number of cards in deck
    auto* mngr = Game::Instance()->get_mngr();
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto _m_deck = mngr->getComponent<Deck>(player);
    auto& pDeck = _m_deck->card_names();
    if (pDeck.size() < 8)
    {
        _activate_confirm_button = true;
        _activate_exchange_button = false;
    }
    else if (pDeck.size() >= 8 && pDeck.size() < 10)
    {
        _activate_confirm_button = true;
        _activate_exchange_button = true;
    }
    else if (pDeck.size() == 10)
    {
        _activate_confirm_button = false;
        _activate_exchange_button = true;
    }
}

void RewardScene::create_reward_exchange_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();

    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at("initial_info"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );
    imgComp->swap_textures();
    mngr->setHandler(ecs::hdlr::CONFIRMREWARD, e);
    buttonComp->connectClick([buttonComp, this, imgComp] {
        //we only select a reward if previously we have chosen something
        if (_lr != nullptr && !_selected) {
            _lr->apply_filter(255, 255, 255);
            _lr->swap_textures();
            _selected = true;

            ////We get the reference to the deck
            //auto* mngr = Game::Instance()->get_mngr();
            //auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
            //auto _m_deck = mngr->getComponent<Deck>(player);

            ////Removes old card from deck
            //exchange_deck_card();

            ////We add the new card to the deck
            //_m_deck->add_card_to_deck(new Fireball());

            ////Refresh deck
            //auto& pDeck = _m_deck->card_names();
            //refresh_my_deck_cards(pDeck);

            //imgComp->swap_textures();
        }
        });
    buttonComp->connectHover([buttonComp, imgComp, this]() {
        if (_selected) return;
        std::cout << "hover -> Reward exchange button: " << std::endl;
        //filter
        imgComp->apply_filter(128, 128, 128);
        });
    buttonComp->connectExit([buttonComp, imgComp, this]() {
        std::cout << "exit -> Reward exchange button: " << std::endl;
        //filter
        imgComp->apply_filter(255, 255, 255);
        });
}

void RewardScene::update(uint32_t delta_time) {
    Scene::update(delta_time);

    if (!_activate_confirm_button && !_activate_exchange_button && _lr != nullptr) {
        auto* mngr = Game::Instance()->get_mngr();
        auto imgCompConfirm = mngr->getComponent<ImageForButton>(mngr->getHandler(ecs::hdlr::CONFIRMREWARD));
        imgCompConfirm->swap_textures();

        //Checks number of cards to activate or desactivate buttons
        //Changes true or false in "comfirm" and "exchange"
        check_number();
    }
}