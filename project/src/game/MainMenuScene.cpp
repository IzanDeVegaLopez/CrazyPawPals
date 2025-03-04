#include "MainMenuScene.h"
#include "../our_scripts/components/Button.h"
#include "GameStructs.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../sdlutils/Texture.h"
MainMenuScene::MainMenuScene() : Scene(ecs::scene::MAINMENUSCENE)
{
	auto* mngr = Game::Instance()->get_mngr();
	_background = &sdlutils().images().at("start");
    GameStructs::ButtonProperties buttonPropTemplate = { {550, 200},
       500.0f, 125.0f, 0.0f, ""
    };
    GameStructs::ButtonProperties startB = buttonPropTemplate;
    startB.sprite_key = "play_button";
    create_start_button(startB);
}

MainMenuScene::~MainMenuScene()
{

}

void MainMenuScene::initScene()
{

}

void MainMenuScene::enterScene()
{

}

void MainMenuScene::exitScene()
{

}
void MainMenuScene::render() {
	_background->render(0, 0);
	Scene::render();
}
void MainMenuScene::create_start_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    buttonComp->connectClick([buttonComp, &mngr]() {
        if (buttonComp->clicked()) return;
        buttonComp->set_clicked(true);
        //std::cout << "left click-> button" << std::endl;
        Game::Instance()->change_Scene(Game::SELECTIONMENU);
        });

    buttonComp->connectHover([buttonComp]() {
        (void)buttonComp;
        });
}
