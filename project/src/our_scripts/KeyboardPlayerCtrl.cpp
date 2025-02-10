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

    //suponiendo que luego cambiare vel por dir probablemente
    if (ihdlr.keyDownEvent()) {
        auto& vel = o->getVel();

        if (ihdlr.isKeyDown(_left)) {
            vel.setX(-1);
        }
        else if (ihdlr.isKeyDown(_right)) {
            vel.setX(1);
        }
        else {
            vel.setX(0); //stop x axis movement
        }

        if (ihdlr.isKeyDown(_up)) {
            vel.setY(-1);
        }
        else if (ihdlr.isKeyDown(_down)) {
            vel.setY(1);
        }
        else {
            vel.setY(0); //stop y axis movement
        }

        //reload
        if (ihdlr.isKeyDown(_reload)) {
            std::cout << "recarga" << std::endl;
        }

        //collect
        if (ihdlr.isKeyDown(_collect)) {
            //if we are not close enought to a reward, do nothing
            std::cout << "colecta" << std::endl;
        }


    }
}