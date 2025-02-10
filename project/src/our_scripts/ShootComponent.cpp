#include "ShootComponent.h"
#include "Bullet.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include <iostream>

ShootComponent::ShootComponent(float shootCoolDown) : _shootCoolDown(shootCoolDown), _lastShoot(sdlutils().currRealTime()) {}

void ShootComponent::handleInput(Container* o) {

    auto& ihdlr = ih();

    if (ihdlr.mouseButtonEvent() && ihdlr.getMouseButtonState(InputHandler::LEFT) && sdlutils().currRealTime() >= _lastShoot+_shootCoolDown) {
        std::cout << "disparar" << std::endl;

        Vector2D mousePos(ih().getMousePos().first, ih().getMousePos().second);
        Vector2D playerPos = o->getPos();
        Vector2D direction = (mousePos - playerPos).normalize();

        std::cout << direction << std::endl;

        Bullet* bullet = new Bullet(playerPos, direction, 5.0f);
        _lastShoot = sdlutils().currRealTime();
    }
}