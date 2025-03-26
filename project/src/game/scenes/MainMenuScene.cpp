#include "MainMenuScene.h"
#include "../../our_scripts/components/ui/Button.h"
#include "../../our_scripts/components/rendering/transformless_dyn_image.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/Texture.h"
MainMenuScene::MainMenuScene() : Scene(ecs::scene::MAINMENUSCENE)
{
	auto* mngr = Game::Instance()->get_mngr();

	_background = &sdlutils().images().at("start");

    GameStructs::ButtonProperties buttonPropTemplate = { 
        { {0.5f, 0.5f},{0.6f, 0.2f} },
        0.0f, ""
    };

    //Boton start
    GameStructs::ButtonProperties startB = buttonPropTemplate;
    startB.sprite_key = "enter_game";
    create_start_button(startB);

    //Boton controls
    buttonPropTemplate.rect.position.y += 0.3f;
    GameStructs::ButtonProperties controlsB = buttonPropTemplate;
    controlsB.sprite_key = "controls_button";
    create_controls_button(controlsB);

    //Boton exit
    buttonPropTemplate.rect.position.y += 0.3f;
    GameStructs::ButtonProperties exitB = buttonPropTemplate;
    exitB.sprite_key = "exit_game";
    create_exit_button(exitB);
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
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    buttonComp->connectClick([buttonComp, imgComp, mngr]() {
        //std::cout << "left click-> button" << std::endl;
        Game::Instance()->change_Scene(Game::SELECTIONMENU);
        imgComp->apply_filter(255, 255, 255);
        });

    buttonComp->connectHover([buttonComp, imgComp]() {
        imgComp->apply_filter(128, 128, 128);
        });
    buttonComp->connectExit([buttonComp, imgComp]() {
        imgComp->apply_filter(255, 255, 255);
        });
}

void MainMenuScene::create_controls_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    buttonComp->connectClick([buttonComp, imgComp, mngr]() {
        //std::cout << "left click -> controls button" << std::endl;

        Game::Instance()->change_Scene(Game::CONTROLSSCENE);
        imgComp->apply_filter(255, 255, 255);
        });

    buttonComp->connectHover([buttonComp, imgComp]() {
        imgComp->apply_filter(128, 128, 128);
        });
    buttonComp->connectExit([buttonComp, imgComp]() {
        imgComp->apply_filter(255, 255, 255);
        });
}

void MainMenuScene::create_exit_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    buttonComp->connectClick([buttonComp, imgComp, mngr]() {
        //std::cout << "left click -> exit button" << std::endl;

        Game::Instance()->set_exit(true);
        imgComp->apply_filter(255, 255, 255);
    });

    buttonComp->connectHover([buttonComp, imgComp]() {
        imgComp->apply_filter(128, 128, 128);
        });
    buttonComp->connectExit([buttonComp, imgComp]() {
        imgComp->apply_filter(255, 255, 255);
        });
}
