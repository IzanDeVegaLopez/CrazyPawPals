#include "MythicScene.h"

#include "../../our_scripts/components/ui/Button.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"

#include "../../our_scripts/components/rendering/ImageForButton.h"
#include "../../our_scripts/mythic/MythicItems.h"
#include "../../our_scripts/mythic/MythicItem.h"
#include "../../our_scripts/mythic/MythicDataComponent.h"
#include "../../our_scripts/components/MythicComponent.h"

#include <iostream>

MythicScene::MythicScene() : Scene(ecs::scene::MYTHICSCENE), _lr(nullptr), 
_selected(false), _activate_confirm_button(false), _chosen_mythic(nullptr)
{
}

MythicScene::~MythicScene()
{
}

void MythicScene::initScene()
{
    //Background
    create_static_background(&sdlutils().images().at("reward"));
    //Reward Buttons
    create_reward_buttons();
    //Mythics
    //create_my_mythic();
}

void MythicScene::enterScene()
{
    ecs::Manager* mngr = Game::Instance()->get_mngr();
    ecs::entity_t player = mngr->getHandler(ecs::hdlr::PLAYER);
    mngr->addComponent<MythicComponent>(player); //QUITAR. QUE SE ANADA CUANDO SE CREE PLAYER
    MythicComponent* _m_mythics = mngr->getComponent<MythicComponent>(player);
    std::vector<MythicItem*> pMythics = _m_mythics->get_mythics();
    //refresh_my_mythic(pMythics);
    refresh_mythics();
}

void MythicScene::exitScene()
{
}

void MythicScene::update(uint32_t delta_time)
{
    Scene::update(delta_time);
    if (!_activate_confirm_button && _lr != nullptr) {
        auto mngr = Game::Instance()->get_mngr();
        auto imgCompConfirm = mngr->getComponent<ImageForButton>(mngr->getHandler(ecs::hdlr::CONFIRMREWARD));
        imgCompConfirm->swap_textures();
        _activate_confirm_button = true;
    }
}

ecs::entity_t MythicScene::create_mythic_button(const GameStructs::ButtonProperties& bp)
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
    auto data = mngr->addComponent<MythicDataComponent>(e, bp.sprite_key);

    buttonComp->connectClick([buttonComp, imgComp, this, e]() {
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
            _chosen_mythic = e;
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

    return e;
}

void MythicScene::create_my_mythic()
{
    auto* mngr = Game::Instance()->get_mngr();

    //Get Player´s mythics
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    if (player && !mngr->hasComponent<MythicComponent>(player)) {
        //when we add these entities, our olayer doesnt have any deck as component
        mngr->addComponent<MythicComponent>(player);
    }

    MythicComponent* _m_mythics = mngr->getComponent<MythicComponent>(player);
    std::vector<MythicItem*> pMythics = _m_mythics->get_mythics();

    for (MythicItem* mi : pMythics) {

    }

    //float umbral = 0.095f;
    //auto iterator = _m_mythics->all_cards().card_list().begin();
    GameStructs::ButtonProperties{

    };

   /* for (const auto& it : pDeck) {
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
    }*/
}

void MythicScene::create_a_mythic(const GameStructs::CardButtonProperties& bp)
{
}

//void MythicScene::refresh_my_mythic(const std::vector<MythicItem*> ml)
//{
//    //auto* mngr = Game::Instance()->get_mngr();
//    //auto infos = mngr->getEntities(ecs::grp::REWARDDECK);
//
//    //// Obtener la referencia al mazo actualizado
//    //auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
//    //auto* deck = mngr->getComponent<Deck>(player);
//    //auto& updatedCardList = deck->all_cards().card_list(); // Obtener los nuevos punteros
//
//    //auto itInfo = infos.begin();
//    //auto itCard = updatedCardList.begin();
//    //auto it = cl.begin();
//
//    //// Refrescar las texturas y actualizar punteros
//    //for (; it != cl.end() && itCard != updatedCardList.end(); ++it, ++itInfo, ++itCard) {
//    //    auto img = mngr->getComponent<transformless_dyn_image>(*itInfo);
//    //    img->set_texture(&sdlutils().images().at(*it));
//
//    //    // Actualizar la referencia a la carta en el boton
//    //    auto buttonComp = mngr->getComponent<Button>(*itInfo);
//    //    if (buttonComp) {
//    //        static_cast<CardButton*>(buttonComp)->set_it(*itCard); // Metodo para actualizar puntero
//    //    }
//    //}
//
//    //// Rellenar con imagenes vacias si hay menos cartas en el nuevo mazo
//    //for (; itInfo != infos.end(); ++itInfo) {
//    //    auto img = mngr->getComponent<transformless_dyn_image>(*itInfo);
//    //    img->set_texture(&sdlutils().images().at("initial_info"));
//
//    //    auto buttonComp = mngr->getComponent<Button>(*itInfo);
//    //    if (buttonComp) {
//    //        static_cast<CardButton*>(buttonComp)->set_it(nullptr); // Poner puntero a nullptr si no hay carta
//    //    }
//    //}
//}

std::string MythicScene::select_mythic(GameStructs::MythicType mt)
{
    std::string s = "";
    switch (mt)
    {
    case GameStructs::BLOODCLAW: s = "mythic_bloodclaw";
        break;
    case GameStructs::MANASWAP: s = "mythic_manaswap";
        break;
    case GameStructs::SHIELDHARVEST: s = "mythic_shieldharvest";
        break;
    case GameStructs::MANACATALYST: s = "mythic_manacatalyst";
        break;
    default:
        break;
    }
    std::cout << "mythic: " + s << std::endl;
    return s;
}

std::pair<std::string, GameStructs::MythicType> MythicScene::get_unique_mythic(std::unordered_set<std::string>& appeared_mythic)
{
    std::string sprite;
    GameStructs::MythicType mt;
    //Selects a unique object 
    do {
        mt = (GameStructs::MythicType)sdlutils().rand().nextInt(0, GameStructs::LAST_MYTHIC);
        sprite = select_mythic(mt);
    } while (appeared_mythic.find(sprite) != appeared_mythic.end());

    appeared_mythic.insert(sprite);
    return { sprite, mt };
}

void MythicScene::create_reward_buttons()
{
    float umbral = 0.2f;
    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.45f, 0.1f}, {0.125f, 0.2f} },
        0.0f, "", ecs::grp::MYTHICOBJS
    };

    // Auxiliar set to check if we repeat some card
    std::unordered_set<std::string> appeared_myhtic;

    //three card reward buttons
    buttonPropTemplate.sprite_key = get_unique_mythic(appeared_myhtic).first;
    create_mythic_button(buttonPropTemplate);

    buttonPropTemplate.sprite_key = get_unique_mythic(appeared_myhtic).first;
    buttonPropTemplate.rect.position.x -= umbral;
    create_mythic_button(buttonPropTemplate);

    buttonPropTemplate.sprite_key = get_unique_mythic(appeared_myhtic).first;
    buttonPropTemplate.rect.position.x += umbral * 2;
    create_mythic_button(buttonPropTemplate);

    ////selected button
    //buttonPropTemplate.ID = ecs::grp::UI;
    //buttonPropTemplate.sprite_key = "confirm_reward";
    //buttonPropTemplate.rect.position = { 0.35f, 0.35f };
    //buttonPropTemplate.rect.size = { 0.3f, 0.15f };
    //create_reward_selected_button(buttonPropTemplate);

}

void MythicScene::refresh_mythics()
{
    auto* mngr = Game::Instance()->get_mngr();
    auto& mythic_cards = mngr->getEntities(ecs::grp::MYTHICOBJS);
    std::unordered_set<std::string> appeared_mythic;
    //refresh the three reward card button
    for (auto& e : mythic_cards) {
        auto s = get_unique_mythic(appeared_mythic);
        auto img = mngr->getComponent<transformless_dyn_image>(e);
        img->set_texture(&sdlutils().images().at(s.first));
        auto data = mngr->getComponent<MythicDataComponent>(e);
        data->set_data(s.first, s.second);
    }
}

void MythicScene::create_reward_selected_button(const GameStructs::ButtonProperties& bp)
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
            //add_new_reward_card();
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

void MythicScene::create_next_round_button()
{
}

void MythicScene::resize(ImageForButton* im, float factor)
{
    im->destination_rect.size = { im->destination_rect.size.x * factor, im->destination_rect.size.y * factor };
}

