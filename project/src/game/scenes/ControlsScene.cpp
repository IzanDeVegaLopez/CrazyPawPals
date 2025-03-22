#include "ControlsScene.h"
#include "../../our_scripts/components/Button.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/Texture.h"

ControlsScene::ControlsScene() : Scene(ecs::scene::CONTROLSSCENE)
{
    auto* mngr = Game::Instance()->get_mngr();

    _background = &sdlutils().images().at("controls");

    //Boton back to main menu
    GameStructs::ButtonProperties mainMenuB = {
        { {0.1f, 0.1f},{0.1f, 0.1f} },
        0.0f, ""
    };
    mainMenuB.sprite_key = "heart";
    create_mainmenu_button(mainMenuB);

}

ControlsScene::~ControlsScene()
{

}

void ControlsScene::initScene()
{

}

void ControlsScene::enterScene()
{

}

void ControlsScene::exitScene()
{

}
void ControlsScene::render() {
    _background->render(200, 300);
    Scene::render();
}

void ControlsScene::create_mainmenu_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    buttonComp->connectClick([buttonComp, &mngr]() {
        std::cout << "left click -> main menu button" << std::endl;
        Game::Instance()->change_Scene(Game::MAINMENU);
        });

    buttonComp->connectHover([buttonComp]() {
        (void)buttonComp;
        });
}