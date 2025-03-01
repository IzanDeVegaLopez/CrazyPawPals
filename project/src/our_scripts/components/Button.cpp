#include "Button.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Manager.h"
#include "../../game/Game.h"
#include "Transform.h"
#include <iostream>

using namespace ecs;

Button::Button(): _current_state(EMPTY), _clicked(false) {
}

Button::~Button() {}

void Button::initComponent() {
    auto* mngr = Game::Instance()->get_mngr();
    auto tr = mngr->getComponent<Transform>(_ent);
    assert(tr != nullptr);

    _button_collider = { (int)tr->getPos().getX(), (int)tr->getPos().getY(), (int)tr->getWidth(), (int)tr->getHeight() };
}

void Button::update(uint32_t delta_time) {
    if (mouseOver()) {
        if (_current_state != HOVER) {
            _current_state = HOVER;
            emitHover(); 
        }
    }
    else {
        _current_state = EMPTY;
    }

    if (ih().getMouseButtonState(InputHandler::LEFT)) {
        leftClickDown();
    }
    else {
        leftClickUp();
    }
}

void Button::leftClickUp() {
    _current_state = EMPTY;
}

void Button::leftClickDown() {
    if (mouseOver() && _current_state == HOVER) {
        _current_state = CLICK;
        emitClick(); 
    }
}

bool Button::mouseOver() {
    Vector2D mousePos(ih().getMousePos().getX(), ih().getMousePos().getY());
    SDL_Rect mouseRect = build_sdlrect(mousePos, 1, 1);
    return SDL_HasIntersection(&_button_collider, &mouseRect);
}


void Button::connectClick(SDLEventCallback callback) {
    _click_callbacks.push_back(callback);
}


void Button::connectHover(SDLEventCallback callback) {
    _hover_callbacks.push_back(callback);
}


void Button::emitClick() const {
    for (const auto& callback : _click_callbacks) {
        callback();
    }
}


void Button::emitHover() const {
    for (const auto& callback : _hover_callbacks) {
        callback();
    }
}