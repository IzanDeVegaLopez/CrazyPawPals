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
#include "../../our_scripts/components/rendering/transformless_dyn_image.h" 
#include "../../our_scripts/components/rendering/ImageForButton.h"
#include "../../our_scripts/components/cards/RewardDataComponent.h"
#include "../../our_scripts/components/Health.h" 
#ifdef GENERATE_LOG
#include "../../our_scripts/log_writer_to_csv.hpp"
#endif

#include <iostream>

RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE),_selected_card(nullptr),
_heal(false), _lr(nullptr),
_selected(false), _activate_confirm_button(false), _chosen_card(nullptr), _activate_exchange_button(false), _last_deck_card_img(nullptr), _activate_heal(false), _exchange(false)
{
}
RewardScene::~RewardScene()
{
}

void RewardScene::initScene() {
    create_static_background(&sdlutils().images().at("reward"));
    create_reward_info();
    create_reward_buttons();
    create_my_deck_cards();
}

void RewardScene::enterScene()
{
    auto* mngr = Game::Instance()->get_mngr();
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto _m_deck = mngr->getComponent<Deck>(player);
    auto& draw = _m_deck->move_discard_to_draw().card_list();
    refresh_my_deck_cards(draw);
    refresh_rewards();
    check_number();
    Game::Instance()->get_mngr()->change_ent_scene(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA), ecs::scene::REWARDSCENE);
#ifdef GENERATE_LOG
    log_writer_to_csv::Instance()->add_new_log();
    log_writer_to_csv::Instance()->add_new_log("ENTERED REWARDS SCENE");
#endif
}

void RewardScene::exitScene()
{
    change_pos(false);
    auto* mngr = Game::Instance()->get_mngr();
    auto cb = mngr->getHandler(ecs::hdlr::CONFIRMREWARD);
    auto eb = mngr->getHandler(ecs::hdlr::EXCHANGEBUTTON);

    auto cImg = mngr->getComponent<ImageForButton>(cb);

    cImg->destination_rect.position = { 0.438f, 0.55f };
    cImg->_filter = false;
    cImg->swap_textures();
    cImg->_filter = false;

    if (!_exchange && _last_deck_card_img != nullptr) {
        auto eImg = mngr->getComponent<ImageForButton>(eb);
        _last_deck_card_img->destination_rect.position.y += 0.05f;
        eImg->_filter = false;
        eImg->swap_textures();
        eImg->_filter = false;
    }
    else if (_exchange) {
        auto eImg = mngr->getComponent<ImageForButton>(eb);
        eImg->destination_rect.position.x = 1.1f;
        eImg->_filter = false;
        eImg->swap_textures();
        eImg->_filter = false;
    }

    _lr->destination_rect.size = { _lr->destination_rect.size.x /1.1f,  _lr->destination_rect.size.y/1.1f };
    
    _exchange = false;
    _lr = nullptr;
    _selected = false;
    _heal = false;

    _last_deck_card_img = nullptr;
    _selected_card = nullptr;

    _chosen_card = nullptr;

    _activate_confirm_button = false;
    _activate_exchange_button = false;
    _activate_heal = false;
#ifdef GENERATE_LOG
    log_writer_to_csv::Instance()->add_new_log("EXIT REWARDS SCENE");
    log_writer_to_csv::Instance()->add_new_log();
#endif
}

//method to get a unique card (used to prevent repeated rewards)
std::pair<std::string, GameStructs::CardType> RewardScene::get_unique_card(std::unordered_set<std::string>& appeared_cards) {
    std::string sprite;
    GameStructs::CardType ct;
    do {
        ct = (GameStructs::CardType)sdlutils().rand().nextInt(0, GameStructs::LAST_CARD);
        sprite = select_card(ct);
    } while (appeared_cards.find(sprite) != appeared_cards.end()); // Repite si la carta ya apareci�

    appeared_cards.insert(sprite);
    return { sprite, ct };
}

//method to return a string used for the render part
std::string RewardScene::select_card(GameStructs::CardType ct) {
    std::string s = "";
    switch (ct)
    {
    case GameStructs::FIREBALL: s = "card_fireball";
        break;
    case GameStructs::LIGHTING: s = "card_lighting";
        break;
    case GameStructs::KUNAI: s = "card_kunai";
        break;
    case GameStructs::RECOVER: s = "card_recover";
        break;
    case GameStructs::MINIGUN: s = "card_minigun";
        break;
    case GameStructs::SPRAY: s = "card_cardSpray";
        break;
    case GameStructs::ELDRITCH_BLAST: s = "card_eldritchBlast";
        break;
    case GameStructs::COMMUNE: s = "card_commune";
        break;
    case GameStructs::EVOKE: s = "card_evoke";
        break;
    case GameStructs::QUICK_FEET: s = "card_quickFeet";
        break;
    default:
        break;
    }
    //std::cout << "reward card: " + s << std::endl;
    return s;
}

void RewardScene::refresh_rewards() {
    auto* mngr = Game::Instance()->get_mngr();
    auto& rewards_cards = mngr->getEntities(ecs::grp::REWARDCARDS);
    std::unordered_set<std::string> appeared_cards;
    //refresh the three reward card button
    for (auto& e : rewards_cards) {
        auto s = get_unique_card(appeared_cards);
        auto img = mngr->getComponent<transformless_dyn_image>(e);
        img->set_texture(&sdlutils().images().at(s.first));
        auto data = mngr->getComponent<RewardDataComponent>(e);
        data->set_data(s.first, s.second);
    }

    change_pos(true);
}

void RewardScene::change_pos(bool enter) {
    //auxiliar lambda function to swap positions
    auto swap_positions = [](transformless_dyn_image* img1, transformless_dyn_image* img2) {
        auto aux = img1->destination_rect.position.x;
        img1->destination_rect.position.x = img2->destination_rect.position.x;
        img2->destination_rect.position.x = aux;
        };

#pragma region obtain references
    auto* mngr = Game::Instance()->get_mngr();
    //third reward
    auto& rewardCard = mngr->getEntities(ecs::grp::REWARDCARDS)[2];

    // players health
    auto playerHealth = mngr->getComponent<Health>(mngr->getHandler(ecs::hdlr::PLAYER));
    int act = playerHealth->getHealth();
    int max = playerHealth->getMaxHealth();

    // other references
    auto healthReward = mngr->getHandler(ecs::hdlr::REWARDHEALTH);
    auto img = mngr->getComponent<transformless_dyn_image>(rewardCard); // �ltima carta
    auto healImg = mngr->getComponent<transformless_dyn_image>(healthReward);

    auto bRewardButton = mngr->getComponent<Button>(rewardCard);
    auto hRewardButton = mngr->getComponent<Button>(healthReward);
#pragma endregion

#pragma region health reward activation

    if (enter) { //if we need to activate the heal reward
        if ((float)act / (float)max <= 0.2f) {
            swap_positions(img, healImg);
            _activate_heal = true;
        }
    }
    else { //in other case, the condition to swap changes
        //we only swap if player has chosen heal reward;
        if (_activate_heal)swap_positions(img, healImg);
    }
#pragma endregion
}

void RewardScene::create_reward_buttons() {
    float umbral = 0.2f;
    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.45f, 0.115f}, {0.125f, 0.2f} },
        0.0f, "", ecs::grp::REWARDCARDS
    };

    // Auxiliar set to check if we repeat some card
    std::unordered_set<std::string> appeared_cards;

    //three card reward buttons
    buttonPropTemplate.sprite_key = get_unique_card(appeared_cards).first;
    create_reward_card_button(buttonPropTemplate);

    buttonPropTemplate.sprite_key = get_unique_card(appeared_cards).first;
    buttonPropTemplate.rect.position.x -= umbral;
    create_reward_card_button(buttonPropTemplate);
     
    buttonPropTemplate.sprite_key = get_unique_card(appeared_cards).first;
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
    buttonPropTemplate.rect.position = { 0.438f, 0.55f };
    buttonPropTemplate.rect.size = { 0.15f, 0.075f };
    create_reward_selected_button(buttonPropTemplate);

    //exchange button
    buttonPropTemplate.ID = ecs::grp::UI;
    buttonPropTemplate.sprite_key = "exchange_reward";
    buttonPropTemplate.rect.position.x = 1.1f;
    create_reward_exchange_button(buttonPropTemplate);

    ////next round button
    //buttonPropTemplate.ID = ecs::grp::UI;
    //buttonPropTemplate.sprite_key = "enter_game";
    //buttonPropTemplate.rect.position = { 2.0f, 0.35f };
    //create_next_round_button(buttonPropTemplate);
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
    mngr->addComponent<RewardDataComponent>(e, bp.sprite_key);
    auto ri = mngr->getComponent<transformless_dyn_image>(mngr->getHandler(ecs::hdlr::REWARDINFO));

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
            _heal = true;
        }
    });
    buttonComp->connectHover([mngr, buttonComp, imgComp, ri, e,this]() {
        if (_selected) return;
        //std::cout << "hover -> Reward button: " << std::endl;
        //filter
        imgComp->_filter = true;
        auto& sp = mngr->getComponent<RewardDataComponent>(e)->sprite();
        ri->set_texture(&sdlutils().images().at(sp + "_info"));
    });
    buttonComp->connectExit([buttonComp, imgComp, ri]() {
        //std::cout << "exit -> Reward button: " << std::endl;
        //filter
        imgComp->_filter = false;
        ri->set_texture(&sdlutils().images().at("initial_info"));
        
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
    //used to get the sprite key for show a specific info
    mngr->addComponent<RewardDataComponent>(e, bp.sprite_key);
    auto ri = mngr->getComponent<transformless_dyn_image>(mngr->getHandler(ecs::hdlr::REWARDINFO));

    buttonComp->connectClick([buttonComp, imgComp, this, e]() {
        if (_selected) {
            //std::cout << "already selected" << std::endl;
        }
        else {
            //std::cout << "left click -> reward card button" << std::endl;
            if (_lr == nullptr) {
                resize(imgComp, 1.1f);
            }
            else if (_lr != nullptr && _lr != imgComp) {
                resize(imgComp, 1.1f);
                resize(_lr, 1.0f / 1.1f);
            }
            _lr = imgComp;
            _chosen_card = e;
            _heal = false;
        }
    });
    buttonComp->connectHover([mngr,buttonComp, imgComp, ri, e,this]() {
        if (_selected) return;
        //std::cout << "hover -> Reward button: " << std::endl;
        //filter
        imgComp->_filter = true;
        auto& sp = mngr->getComponent<RewardDataComponent>(e)->sprite();
        ri->set_texture(&sdlutils().images().at(sp + "_info"));

        });
    buttonComp->connectExit([buttonComp, imgComp, ri]() {
        //std::cout << "exit -> Reward button: " << std::endl;
        //filter
        imgComp->_filter = false;
        ri->set_texture(&sdlutils().images().at("initial_info"));
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
    auto buttonComp = mngr->getComponent<CardButton>(e);
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    buttonComp->connectClick([buttonComp, imgComp, this, bp] {
        if (_selected || _heal) return;
        imgComp->destination_rect.size = { imgComp->_original_w,  imgComp->_original_h };
        auto it = buttonComp->It();
        //only assign a valid iterator
        if (it != _selected_card) {

            _selected_card = it;
            imgComp->destination_rect.position.y -= 0.05f;
            if (_last_deck_card_img != nullptr) _last_deck_card_img->destination_rect.position.y += 0.05f;
            _last_deck_card_img = static_cast<ImageForButton*>(imgComp);
            //std::cout << "card selected: "<< std::endl;
        }
        });

    buttonComp->connectHover([buttonComp, imgComp, this]() {
        //std::cout << "hover -> Reward button: " << std::endl;
        //filter
        if (_selected) return;
        imgComp->_filter = true;
        /*imgComp->destination_rect.position.y -= 0.125f;*/
       /* imgComp->destination_rect.size = { imgComp->destination_rect.size.x * 1.25f,  imgComp->destination_rect.size.y * 1.25f };*/
        });
    buttonComp->connectExit([buttonComp, imgComp]() {
        //std::cout << "exit -> Reward button: " << std::endl;
        /*imgComp->destination_rect.position.y += 0.125f;*/
        //filter
        imgComp->_filter = false;
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
    auto& pDeck = _m_deck->move_discard_to_draw(true).card_list();

    float umbral = 0.095f;
    auto iterator = pDeck.begin();
    GameStructs::CardButtonProperties propTemplate = {
        { {0.01f, 0.8f}, {0.1f, 0.175f} },
        0.0f, "", ecs::grp::REWARDDECK, *iterator
    };

    for (const auto& it : pDeck) {
#pragma region convert a class name to a string
        std::string typeName = it->get_name();
        /* Unnecessary
        std::string prefix = "class ";
        if (typeName.find(prefix) == 0) {  // Si empieza con "class "
            typeName = typeName.substr(prefix.size());  // Elimina "class "
            typeName[0] = tolower(typeName[0]);
        }
        */
#pragma endregion
        propTemplate.sprite_key = /*"card_"+*/typeName;
        create_a_deck_card(propTemplate);
        propTemplate.rect.position.x += umbral;
        iterator++;
    }
    propTemplate.iterator = nullptr;
    for (int i = 0; i < 4; ++i) {
        propTemplate.sprite_key = "initial_info";
        create_a_deck_card(propTemplate);
        propTemplate.rect.position.x += umbral;
    }
}

void RewardScene::refresh_my_deck_cards(const std::list<Card*>& cl) {
    auto* mngr = Game::Instance()->get_mngr();
    auto infos = mngr->getEntities(ecs::grp::REWARDDECK);

    auto itRewardInfo = infos.begin();

    //refresh my deck info and represent it
    for (auto& c : cl) {
        //obtain each ones component
        auto img = mngr->getComponent<transformless_dyn_image>(*itRewardInfo);
       
        std::string typeName = c->get_name();

        //change to the newest texture
        img->set_texture(&sdlutils().images().at(typeName));
        //Refresh the pointer saved in the component
        auto buttonComp = mngr->getComponent<Button>(*itRewardInfo);
        if (buttonComp) {
            static_cast<CardButton*>(buttonComp)->set_it(c);
        }
        ++itRewardInfo;
    }

    //refresh rest of the deck infos (blank infos)
    for (; itRewardInfo != infos.end(); ++itRewardInfo) {
        auto img = mngr->getComponent<transformless_dyn_image>(*itRewardInfo);
            img->set_texture(&sdlutils().images().at("initial_info"));

            auto buttonComp = mngr->getComponent<Button>(*itRewardInfo);
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
        if (_heal) {
            auto* mngr = Game::Instance()->get_mngr();
            auto player = mngr->getHandler(ecs::hdlr::PLAYER);
            auto phealth = mngr->getComponent<Health>(player);
            //heal a 50%
            int hn = phealth->getMaxHealth() * 5 / 10;
            phealth->heal(hn);
            _selected = true;
        }
        //we only select a reward if previously we have chosen something
        else if (_lr != nullptr && !_selected) {
            imgComp->_filter = false;
            _lr->swap_textures();
            _selected = true;
            add_new_reward_card();
        }
        Game::Instance()->change_Scene(Game::GAMESCENE);
    });
    buttonComp->connectHover([buttonComp, imgComp, this]() {
        if (_selected) return;
        //std::cout << "hover -> Reward selected button: " << std::endl;
        //filter
        imgComp->_filter = true;
    });
    buttonComp->connectExit([buttonComp, imgComp, this]() {
        //std::cout << "exit -> Reward selected button: " << std::endl;
        //filter
        imgComp->_filter = false;
    });
}

void RewardScene::remove_deck_card()
{
    auto mngr = Game::Instance()->get_mngr();
    auto player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto deck = mngr->getComponent<Deck>(player);
    deck->remove_card(_selected_card);
}

void RewardScene::add_new_reward_card() {
    assert(_chosen_card != nullptr);

    //Once reward is picked, checks number of cards in deck
    auto* mngr = Game::Instance()->get_mngr();
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto _m_deck = mngr->getComponent<Deck>(player);
    //We add the cards to the deck
    Card* c = nullptr;
    GameStructs::CardType ct = mngr->getComponent<RewardDataComponent>(_chosen_card)->CT();
    switch (ct)
    {
    case GameStructs::FIREBALL:
        c = new Fireball();
        break;
    case GameStructs::LIGHTING:
        c = new Lighting();
        break;
    case GameStructs::KUNAI:
        c = new Kunai();
        break;
    case GameStructs::RECOVER:
        c = new Recover();
        break;
    case GameStructs::MINIGUN:
        c = new Minigun();
        break;
    case GameStructs::SPRAY:
        c = new CardSpray();
        break;
    case GameStructs::ELDRITCH_BLAST:
        c = new EldritchBlast();
        break;
    case GameStructs::COMMUNE:
        c = new Commune();
        break;
    case GameStructs::EVOKE:
        c = new Evoke();
        break;
    /*case GameStructs::FULGUR:
        c = new Fulgur();
        break;*/
    case GameStructs::QUICK_FEET:
        c = new QuickFeet();
        break;
    default:
        break;
    }
    if (c != nullptr) {
        _m_deck->add_card_to_deck(c);
        //Refresh deck
        auto& pDeck = _m_deck->move_discard_to_draw().card_list();
        refresh_my_deck_cards(pDeck);
        
    }
}

void RewardScene::check_number()
{
    //Once reward is picked, checks number of cards in deck
    auto* mngr = Game::Instance()->get_mngr();
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto _m_deck = mngr->getComponent<Deck>(player);
    auto& pDeck = _m_deck->move_discard_to_draw().card_list();
    if (pDeck.size() < 6)
    {
        _activate_exchange_button = false;
        _activate_confirm_button = true;
    }
    else if (pDeck.size() >= 6 && pDeck.size() < 8)
    {
        _activate_exchange_button = true;
        _activate_confirm_button = true;
    }
    else if (pDeck.size() == 8)
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
    mngr->setHandler(ecs::hdlr::EXCHANGEBUTTON, e);
    buttonComp->connectClick([buttonComp, this] 
        {
            //if we dont have enough card to exchange, ignore this callback
            if (_selected || _activate_exchange_button || _selected_card == nullptr) return;
            _lr->_filter = false;
            _lr->swap_textures();
            _selected = true;
            _exchange = true;
            remove_deck_card();
            add_new_reward_card();
            _last_deck_card_img->destination_rect.position.y += 0.05f;
            Game::Instance()->change_Scene(Game::GAMESCENE);

        });
    buttonComp->connectHover([buttonComp, imgComp, this]() {
        if (_selected) return;
        imgComp->_filter = true;
        });
    buttonComp->connectExit([buttonComp, imgComp, this]() {
        imgComp->_filter = false;
        });
}

void RewardScene::update(uint32_t delta_time) {
   Scene::update(delta_time);

   if (_activate_confirm_button && _lr != nullptr) {
       auto mngr = Game::Instance()->get_mngr();
       auto imgCompConfirm = mngr->getComponent<ImageForButton>(mngr->getHandler(ecs::hdlr::CONFIRMREWARD));
       imgCompConfirm->swap_textures();
       _activate_confirm_button = false;
   }

   if (_activate_exchange_button && _lr != nullptr && _selected_card != nullptr) {
       auto mngr = Game::Instance()->get_mngr();
       auto imgCompConfirm = mngr->getComponent<ImageForButton>(mngr->getHandler(ecs::hdlr::CONFIRMREWARD));
       auto imgCompExchange = mngr->getComponent<ImageForButton>(mngr->getHandler(ecs::hdlr::EXCHANGEBUTTON));
       imgCompExchange->swap_textures();
       imgCompExchange->destination_rect.position = { imgCompConfirm->destination_rect.position.x - 0.185f, imgCompExchange->destination_rect.position.y};

       imgCompConfirm->destination_rect.position = { imgCompConfirm->destination_rect.position.x + 0.185f, imgCompConfirm->destination_rect.position.y };

       auto buttonC = mngr->getComponent<Button>(mngr->getHandler(ecs::hdlr::CONFIRMREWARD));
       auto buttonE = mngr->getComponent<Button>(mngr->getHandler(ecs::hdlr::EXCHANGEBUTTON));
       _activate_exchange_button = false;
   }

  /* if (_selected) {
       auto mngr = Game::Instance()->get_mngr();
       auto imgCompNext = mngr->getComponent<transformless_dyn_image>(mngr->getHandler(ecs::hdlr::NEXTROUNDBUTTON));
       imgCompNext->destination_rect.position = {0.4f,0.5f};

   }*/
}

void RewardScene::create_next_round_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    mngr->setHandler(ecs::hdlr::NEXTROUNDBUTTON, e);
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    auto buttonComp = mngr->getComponent<Button>(e);

    buttonComp->connectClick([buttonComp, mngr, this]() { if (_selected) Game::Instance()->change_Scene(Game::GAMESCENE); });
    buttonComp->connectHover([buttonComp, imgComp, this]() { imgComp->_filter = true;});
    buttonComp->connectExit([buttonComp, imgComp, this]() { imgComp->_filter = false;});
}

void RewardScene::create_reward_info() {
    auto e = create_entity(ecs::grp::UI,
        _scene_ID,
        new transformless_dyn_image({ { 0.315f,0.335f }, {0.4f,0.2f} },
            0,
            Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam,
            &sdlutils().images().at("initial_info")));
    Game::Instance()->get_mngr()->setHandler(ecs::hdlr::REWARDINFO, e);
}
