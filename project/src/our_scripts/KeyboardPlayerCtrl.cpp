#include "KeyboardPlayerCtrl.h"

#include "../sdlutils/InputHandler.h"
#include "Container.h"
#include "Player.h"

KeyboardPlayerCtrl::KeyboardPlayerCtrl()
    : _left(SDL_SCANCODE_A), _right(SDL_SCANCODE_D), _up(SDL_SCANCODE_W), _down(SDL_SCANCODE_S), 
      _reload(SDL_SCANCODE_SPACE), _collect(SDL_SCANCODE_F) {}

KeyboardPlayerCtrl::~KeyboardPlayerCtrl() {
}

void KeyboardPlayerCtrl::handleInput(Container* o) {
    auto& ihdlr = ih();

    //suponiendo que luego cambiare vel por dir probablemente
    if (ihdlr.keyDownEvent()) { //Si comentamos esta linea (y el cierre de llave abajo) el movimiento parece mas natural
        auto& dir = o->getDir();

        //send direction value
        if (ihdlr.isKeyDown(_left)) {
            dir.setX(-1);
        }
        else if (ihdlr.isKeyDown(_right)) {
            dir.setX(1);
        }
        else {
            dir.setX(0); //stop x axis movement
        }

        if (ihdlr.isKeyDown(_up)) {
            dir.setY(-1);
        }
        else if (ihdlr.isKeyDown(_down)) {
            dir.setY(1);
        }
        else {
            dir.setY(0); //stop y axis movement
        }

        static_cast<Player*>(o)->move();

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
    //shoot
    if (ihdlr.mouseButtonEvent() && ihdlr.getMouseButtonState(InputHandler::LEFT)) {
        //send message to shoot
        Vector2D mousePos = { (float)ihdlr.getMousePos().first, (float)ihdlr.getMousePos().second };
        static_cast<Player*>(o)->shoot(mousePos);
    }
}