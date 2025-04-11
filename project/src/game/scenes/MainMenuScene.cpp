#include "MainMenuScene.h"
#include "../../our_scripts/components/ui/Button.h"
#include "../../our_scripts/components/rendering/transformless_dyn_image.h"
#include "../../our_scripts/components/rendering/ImageForButton.h"

#include "../GameStructs.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/Texture.h"

MainMenuScene::MainMenuScene() : Scene(ecs::scene::MAINMENUSCENE)
{
    create_static_background(&sdlutils().images().at("background"));

    GameStructs::ButtonProperties buttonPropTemplate = { 
        { {0.35f, 0.32f},{0.30f, 0.25f} },
        0.0f, ""
    };

    //Boton start
    GameStructs::ButtonProperties startB = buttonPropTemplate;
    startB.sprite_key = "enter_game";
    create_start_button(startB);

    //Boton controls
    buttonPropTemplate.rect.position.y += 0.18f;
    GameStructs::ButtonProperties controlsB = buttonPropTemplate;
    controlsB.sprite_key = "controls_button";
    create_controls_button(controlsB);

    //Boton exit
    buttonPropTemplate.rect.position.y += 0.18f;
    GameStructs::ButtonProperties exitB = buttonPropTemplate;
    exitB.sprite_key = "exit_game";
    create_exit_button(exitB);
}

MainMenuScene::~MainMenuScene()
{

}

void 
MainMenuScene::initScene()
{

}

void 
MainMenuScene::enterScene()
{
    Game::Instance()->get_mngr()->change_ent_scene(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA), ecs::scene::MAINMENUSCENE);
}

void 
MainMenuScene::exitScene()
{

}

void 
MainMenuScene::create_start_button(const GameStructs::ButtonProperties& bp) {
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);

    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at(bp.sprite_key + "_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
        Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );

    auto buttonComp = mngr->getComponent<Button>(e);
    buttonComp->connectClick([buttonComp, imgComp, mngr]() {
        imgComp->_filter = false;
        imgComp->swap_textures();
        Game::Instance()->change_Scene(Game::SELECTIONMENU);
    });

    buttonComp->connectHover([buttonComp, imgComp]() {
        imgComp->_filter = true;
        imgComp->swap_textures();
    });

    buttonComp->connectExit([buttonComp, imgComp]() {
        imgComp->_filter = false;
        imgComp->swap_textures();
    });
}

void 
MainMenuScene::create_controls_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at(bp.sprite_key + "_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );

    auto buttonComp = mngr->getComponent<Button>(e);
    buttonComp->connectClick([buttonComp, imgComp, mngr]() {
        imgComp->_filter = false;
        imgComp->swap_textures();
        imgComp->_filter = false;
        Game::Instance()->change_Scene(Game::TUTORIAL);
    });

    buttonComp->connectHover([buttonComp, imgComp]() {
        imgComp->_filter = true;
        imgComp->swap_textures();

    });

    buttonComp->connectExit([buttonComp, imgComp]() {
        imgComp->_filter = false;
        imgComp->swap_textures();
    });
}

void 
MainMenuScene::create_exit_button(const GameStructs::ButtonProperties& bp)
{
    auto* mngr = Game::Instance()->get_mngr();
    auto e = create_button(bp);
    auto imgComp = mngr->addComponent<ImageForButton>(e,
        &sdlutils().images().at(bp.sprite_key),
        &sdlutils().images().at(bp.sprite_key + "_selected"),
        bp.rect,
        0,
        Game::Instance()->get_mngr()->getComponent<camera_component>(
            Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
    );

    auto buttonComp = mngr->getComponent<Button>(e);
    
    buttonComp->connectClick([buttonComp, imgComp, mngr]() {
        imgComp->_filter = true;
        imgComp->swap_textures();
        Game::Instance()->set_exit(true);
    });

    buttonComp->connectHover([buttonComp, imgComp]() {
        imgComp->_filter = true;
        imgComp->swap_textures();
    });
    
    buttonComp->connectExit([buttonComp, imgComp]() {
        imgComp->_filter = false;
        imgComp->swap_textures(); 
    });
}