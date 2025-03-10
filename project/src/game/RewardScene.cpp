#include "RewardScene.h"
#include "../our_scripts/components/Button.h"
#include "GameStructs.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"

#include <iostream>
#include <string>


RewardScene::RewardScene() : Scene(ecs::scene::REWARDSCENE)
{
}

RewardScene::~RewardScene()
{
}

void RewardScene::initScene() {

    GameStructs::ButtonProperties buttonPropTemplate = { {sdlutils().width() / 2.0f - 100, sdlutils().height() / 4.0f + 100},
        200.0f, 200.0f, 0.0f, ""
    };

    GameStructs::ButtonProperties reward1B = buttonPropTemplate;
    reward1B.sprite_key = "card_back";
    create_reward_button(reward1B);

    GameStructs::ButtonProperties reward2B = buttonPropTemplate;
    reward2B.sprite_key = "card_back";
    reward2B.pos.setX(2*(sdlutils().width() / 2.0f) - 100);
    create_reward_button(reward2B);

    GameStructs::ButtonProperties reward3B = buttonPropTemplate;
    reward3B.sprite_key = "card_back";
    reward3B.pos.setX(3 * (sdlutils().width() / 2.0f) - 100);
    create_reward_button(reward3B);

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

void RewardScene::create_reward_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();

    //SET TEXTURE

    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    buttonComp->connectClick([buttonComp, &mngr]() {
        if (buttonComp->clicked()) return;
        buttonComp->set_clicked(true);
        std::cout << "left click -> Reward button" << std::endl;
        Game::Instance()->change_Scene(Game::MAINMENU);
        buttonComp->set_clicked(false);
        });

    buttonComp->connectHover([buttonComp]() {
        (void)buttonComp;
        });
}
