#include "KeyboardPlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../game/Game.h"

#include "Deck.hpp"
#include "Transform.h"
#include "ShootComponent.h"
#include "../../ecs/Manager.h"
#include "Health.h"

KeyboardPlayerCtrl::KeyboardPlayerCtrl()
    : _left(SDL_SCANCODE_A), _right(SDL_SCANCODE_D), _up(SDL_SCANCODE_W), _down(SDL_SCANCODE_S), 
      _reload(SDL_SCANCODE_SPACE), _collect(SDL_SCANCODE_F) {}

KeyboardPlayerCtrl::~KeyboardPlayerCtrl() {
}

void 
KeyboardPlayerCtrl::initComponent() {

    _tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
    assert(_tr != nullptr);

    _sc = Game::Instance()->get_mngr()->getComponent<ShootComponent>(_ent);
    assert(_sc != nullptr);

    _dc = Game::Instance()->get_mngr()->getComponent<Deck>(_ent);
    assert(_sc != nullptr);

}

void KeyboardPlayerCtrl::update(Uint32 delta_time) {

    auto& ihdlr = ih();
   if (ihdlr.keyDownEvent()) {
        auto& dir = _tr->getDir();
        //New input
        //Horizontal axis
        dir.setX((ihdlr.isKeyDown(_left) ? -1 : 0) + (ihdlr.isKeyDown(_right) ? 1 : 0));

        //Vertical axis
        dir.setY((ihdlr.isKeyDown(_up) ? -1 : 0) + (ihdlr.isKeyDown(_down) ? 1 : 0));

    //reload
    if (ihdlr.isKeyDown(_reload)) {
        std::cout << "recarga" << std::endl;
        _dc->reload();
    }

        //collect
        if (ihdlr.isKeyDown(_collect)) {
            //if we are not close enought to a reward, do nothing
            std::cout << "colecta" << std::endl;
        }
    }

    if (ihdlr.mouseButtonDownEvent()) {
        //bool leftPressed = ihdlr.getMouseButtonState(InputHandler::LEFT);
        //bool rightPressed = ihdlr.getMouseButtonState(InputHandler::RIGHT);
        Vector2D mousePos = { (float)ihdlr.getMousePos().first, (float)ihdlr.getMousePos().second };
        //shoot
        if (ihdlr.getMouseButtonState(InputHandler::LEFT)) {
            //send message to shoot

            _sc->shoot(mousePos);
        }
        //use card
        else if (ihdlr.getMouseButtonState(InputHandler::RIGHT)) {
            //send message to use a card
            _dc->use_card(mousePos);
        }
        
    }
    
}