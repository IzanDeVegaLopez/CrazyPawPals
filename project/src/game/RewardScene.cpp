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


RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE), _reward_selected(false), _last_selected(nullptr),
_card(false), _health(false), _object(false), _upgrade(false)
{
}

RewardScene::~RewardScene()
{
}

void RewardScene::initScene() {
    float umbral = 0.4f;
    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.5f, 0.2f}, {0.175f, 0.3f} },
        0.0f, ""
    };

    //Selects the cards to show
    select_cards();


    //Reward 1
    GameStructs::ButtonProperties reward1B = buttonPropTemplate;
    reward1B.sprite_key = "reward_life";
    create_reward_button(reward1B);

    //std::cout << reward1B.rect.position.x << std::endl;

    //Reward 2
    GameStructs::ButtonProperties reward2B = buttonPropTemplate;
    reward2B.sprite_key = "reward_el_bl_x2";
    reward2B.rect.position.x += umbral;
    create_reward_button(reward2B);

    //std::cout << reward2B.rect.position.x << std::endl;

    //Reward 3
    GameStructs::ButtonProperties reward3B = buttonPropTemplate;
    reward3B.sprite_key = "reward_card";
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

void RewardScene::select_cards()
{
    //enum all_cards { FIREBALL, LIGHTNING, KUNAI, 
                    // RECOVER, MINIGUN, CARDSPRAY,
                    // ELDRICHBLAST, PRIMORDIA, COMMUNE,
                    // EVOKE, FULGUR, QUICKFEET }; //PRIME(? en revision
                    
    //Picks randomly the cards, between all the enum
    for (int i = 0; i < _cards_vector->size(); i++)
    {
        RandomNumberGenerator rand;
        int index = rand.nextInt(0, 12); 

        switch (index)
        {
        case 0:
            _cards_vector->emplace_back(new Fireball());
            break;
        case 1:
            _cards_vector->emplace_back(new Lighting());
            break;
        case 2:
            _cards_vector->emplace_back(new Kunai());
            break;
        case 3:
            _cards_vector->emplace_back(new Recover());
            break;
        case 4:
            _cards_vector->emplace_back(new Minigun());
            break;
        case 5:
            _cards_vector->emplace_back(new CardSpray());
            break;
        case 6:
            _cards_vector->emplace_back(new EldritchBlast());
            break;
        case 7:
            _cards_vector->emplace_back(new Primordia());
            break;
        case 8:
            _cards_vector->emplace_back(new Commune());
            break;
        case 9:
            _cards_vector->emplace_back(new Evoke());
            break;
        case 10:
            _cards_vector->emplace_back(new Fulgur());
            break;
        case 11:
            _cards_vector->emplace_back(new QuickFeet());
            break;
        default:
            break;
        }


        std::cout << _cards_vector[i].data();

    }
}

void RewardScene::create_reward_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();

    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    //Imagen del boton (seleccionado o sin seleccionar)
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at(bp.sprite_key + "_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );

    buttonComp->connectClick([buttonComp, imgComp, this]() {
        //std::cout << "left click -> Reward button" << std::endl;
        _reward_selected = true;

        //swap the actual buttons textures
        imgComp->swap_textures();

        //register the last clicked button
        if (_last_selected != nullptr && _last_selected != imgComp) {
            _last_selected->swap_textures();
        }
        _last_selected = imgComp;
    });

    buttonComp->connectHover([buttonComp, imgComp]() {
        //std::cout << "hover -> Reward button" << std::endl;
        //filter
        imgComp->apply_filter(128, 128, 128);
    });

    buttonComp->connectExit([buttonComp, imgComp]() {
        //std::cout << "exit -> Reward button" << std::endl;
        //filter
        imgComp->apply_filter(255, 255, 255);
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
