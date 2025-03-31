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
    create_reward_buttons();
    _reward_bg = &sdlutils().images().at("reward");
}

void MythicScene::enterScene()
{
    auto* mngr = Game::Instance()->get_mngr();
    auto* player = mngr->getHandler(ecs::hdlr::PLAYER);
    auto _m_mythics = mngr->getComponent<MythicComponent>(player);
    std::vector<MythicItem*>& pMythics = _m_mythics->get_mythics();
    refresh_my_mythic(pMythics);
    refresh_mythics();
}

void MythicScene::exitScene()
{
}

void MythicScene::render()
{
    _reward_bg->render(0, 0);
    Scene::render();
}

void MythicScene::update(uint32_t delta_time)
{
}

ecs::entity_t MythicScene::create_mythic_button(const GameStructs::ButtonProperties& bp)
{
	return ecs::entity_t();
}

void MythicScene::create_my_mythic()
{
}

void MythicScene::refresh_my_mythic(const std::list<std::string>& ml)
{
}

std::string MythicScene::select_mythic(GameStructs::MythicType Mt)
{
    std::string s = "";
    switch (ct)
    {
    case GameStructs::BLOODCLAW: s = "mythic";
        break;
    case GameStructs::MANASWAP: s = "mythic";
        break;
    case GameStructs::SHIELDHARVEST: s = "mythic";
        break;
    case GameStructs::MANACATALYST: s = "mythic";
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
        sprite = select_card(mt);
    } while (appeared_mythic.find(sprite) != appeared_mythic.end());

    appeared_mythic.insert(sprite);
    return { sprite, mt };
}

void MythicScene::create_reward_buttons()
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
            _chosen_card = e;
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

void MythicScene::refresh_mythics()
{
    auto* mngr = Game::Instance()->get_mngr();
    auto& mythic_cards = mngr->getEntities(ecs::grp::MYTHICOBJS);
    std::unordered_set<std::string> appeared_mythic;
    GameStructs::CardType ct;
    //refresh the three reward card button
    for (auto& e : mythic_cards) {
        auto s = get_unique_card(appeared_mythic);
        auto img = mngr->getComponent<transformless_dyn_image>(e);
        img->set_texture(&sdlutils().images().at(s.first));
        auto data = mngr->getComponent<RewardDataComponent>(e);
        data->set_data(s.first, s.second);
    }

    change_pos(true);
}

