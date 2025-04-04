#include "GameOverScene.h"
#include "../../our_scripts/components/ui/Button.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"

#include "../../our_scripts/components/weapons/player/Revolver.h"
#include "../../our_scripts/components/weapons/player/Rampage.h"
#include "../../our_scripts/components/weapons/player/PumpShotgun.h"
#include "../../our_scripts/components/weapons/player/RampCanon.h"
#include "../../our_scripts/components/weapons/player/Lightbringer.h"

#include "../../our_scripts/card_system/Card.hpp"
#include "../../our_scripts/card_system/PlayableCards.hpp"
#include "../../our_scripts/components/cards/Deck.hpp"
#include "../../our_scripts/components/rendering/Image.h"
#include "../../our_scripts/components/rendering/ImageForButton.h"

#include <iostream>
#include <typeinfo>
#include <algorithm>
GameOverScene::GameOverScene()
    : Scene(ecs::scene::GAMEOVERSCENE) {}

GameOverScene::~GameOverScene()
{
}
void GameOverScene::initScene() {
    create_static_background(&sdlutils().images().at("game_over"));
    create_enter_button();
}
void GameOverScene::enterScene()
{
    Game::Instance()->get_mngr()->change_ent_scene(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA), ecs::scene::GAMEOVERSCENE);
}

void GameOverScene::exitScene()
{
}
void GameOverScene::render() {
    Scene::render();
}
void GameOverScene::create_enter_button() {
    GameStructs::ButtonProperties bp = {
         { {0.4f, 0.5f},{0.3f, 0.125f} },
         0.0f, "enter_game"
    };
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);

    buttonComp->connectClick([buttonComp, mngr, this]() {
        Game::Instance()->change_Scene(Game::MAINMENU);
    }); 
}
