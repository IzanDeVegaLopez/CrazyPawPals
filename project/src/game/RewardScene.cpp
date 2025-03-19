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
    float umbral = 0.4f;
    GameStructs::ButtonProperties buttonPropTemplate = {
        { {0.5f, 0.2f}, {0.175f, 0.3f} },
        0.0f, ""
    };

    GameStructs::ButtonProperties reward1B = buttonPropTemplate;
    reward1B.sprite_key = "card_back";
    create_reward_button(reward1B);

    GameStructs::ButtonProperties reward2B = buttonPropTemplate;
    reward2B.sprite_key = "card_back";
    reward2B.rect.position.x += umbral;
    create_reward_button(reward2B);

    GameStructs::ButtonProperties reward3B = buttonPropTemplate;
    reward3B.sprite_key = "card_back";
    reward3B.rect.position.x -= umbral;
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
        std::cout << "left click -> Reward button" << std::endl;
        Game::Instance()->change_Scene(Game::MAINMENU);
        });

    buttonComp->connectHover([buttonComp]() {
        (void)buttonComp;
        });
}
