#include "KeyboardPlayerCtrl.h"

#include "../sdlutils/InputHandler.h"
#include "Container.h"

KeyboardPlayerCtrl::KeyboardPlayerCtrl()
    : _left(SDL_SCANCODE_A), _right(SDL_SCANCODE_D), _up(SDL_SCANCODE_W), _down(SDL_SCANCODE_S), 
      _reload(SDL_SCANCODE_SPACE), _collect(SDL_SCANCODE_F) {}

KeyboardPlayerCtrl::~KeyboardPlayerCtrl() {
}

void KeyboardPlayerCtrl::handleInput(Container* o) {
    auto& ihdlr = ih();

    if (ihdlr.keyDownEvent()) {
        auto& vel = o->getVel();

        if (ihdlr.isKeyDown(_left)) {
            std::cout << "izquierda" << std::endl;
        }
        else if (ihdlr.isKeyDown(_right)) {
            std::cout << "derecha" << std::endl;
        }
        else {
            vel.setX(0); //stop x axis movement
        }

        if (ihdlr.isKeyDown(_up)) {
            std::cout << "arriba" << std::endl;
        }
        else if (ihdlr.isKeyDown(_down)) {
            std::cout << "abajo" << std::endl;
        }
        else {
            vel.setY(0); //stop y axis movement
        }

        //reload
        if (ihdlr.isKeyDown(_reload)) {
            std::cout << "recarga" << std::endl;
        }

        //collect
        if (ihdlr.isKeyDown(_get_reward)) {
            //if we are not close enought to a reward, do nothing
            std::cout << "colecta" << std::endl;
        }


    }
}