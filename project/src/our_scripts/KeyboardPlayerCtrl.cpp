#include "KeyboardPlayerCtrl.h"

#include "../sdlutils/InputHandler.h"
#include "Container.h"

KeyboardPlayerCtrl::KeyboardPlayerCtrl()
    : _left(SDL_SCANCODE_A), _right(SDL_SCANCODE_D), _up(SDL_SCANCODE_W), _down(SDL_SCANCODE_S) {}

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
            vel.setX(0); // Detener el movimiento horizontal
        }

        if (ihdlr.isKeyDown(_up)) {
            std::cout << "arriba" << std::endl;
        }
        else if (ihdlr.isKeyDown(_down)) {
            std::cout << "abajo" << std::endl;
        }
        else {
            vel.setY(0); // Detener el movimiento vertical
        }
    }
}