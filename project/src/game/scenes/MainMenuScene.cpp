#include "MainMenuScene.h"
#include "../../our_scripts/components/ui/Button.h"
#include "../../our_scripts/components/rendering/transformless_dyn_image.h"
#include "../../our_scripts/components/movement/Transform.h"
#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/Texture.h"
#include "GameScene.h"
MainMenuScene::MainMenuScene() : Scene(ecs::scene::MAINMENUSCENE)
{
	auto* mngr = Game::Instance()->get_mngr();

    create_static_background(&sdlutils().images().at("start"));

    GameStructs::ButtonProperties buttonPropTemplate = { 
        { {0.365f, 0.25f},{0.3f, 0.1625f} },
        0.0f, ""
    };

    //Boton start
    GameStructs::ButtonProperties startB = buttonPropTemplate;
    startB.sprite_key = "enter_game";
    create_start_button(startB);

    //Boton controls
    buttonPropTemplate.rect.position.y += 0.2f;
    GameStructs::ButtonProperties controlsB = buttonPropTemplate;
    controlsB.sprite_key = "controls_button";
    create_controls_button(controlsB);

    //Boton exit
    buttonPropTemplate.rect.position.y += 0.2f;
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
    auto&& manager = *Game::Instance()->get_mngr();
    ecs::entity_t camera = manager.getHandler(ecs::hdlr::CAMERA);
    ecs::entity_t player = manager.getHandler(ecs::hdlr::PLAYER);

    if (player->get_currCmps_size()<=0) {
        player = Game::Instance()->get_gameScene()->create_player();

        if (manager.hasComponent<camera_follow>(camera)) {
            manager.removeComponent<camera_follow>(camera);
            manager.addComponent<camera_follow>(camera, camera_follow_descriptor{
           .previous_position = manager.getComponent<camera_component>(camera)->cam.camera.position,
           .lookahead_time = 1.0f,
           .semi_reach_time = 2.5f
                }, *manager.getComponent<camera_component>(camera), *manager.getComponent<Transform>(player));

        }
    }
    Game::Instance()->get_mngr()->change_ent_scene(camera, ecs::scene::MAINMENUSCENE);
}

void MainMenuScene::exitScene()
{

}
void MainMenuScene::create_start_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto buttonComp = mngr->getComponent<Button>(e);
    auto imgComp = mngr->getComponent<transformless_dyn_image>(e);
    buttonComp->connectClick([buttonComp, imgComp, mngr]() {
        
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
