#include "KeyboardPlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../components/camera_component.hpp"

#include "Deck.hpp"
#include "Transform.h"
#include "Weapon.h"
#include "MovementController.h"
#include "Health.h"

KeyboardPlayerCtrl::KeyboardPlayerCtrl()
    : _left(SDL_SCANCODE_A), _right(SDL_SCANCODE_D), _up(SDL_SCANCODE_W), _down(SDL_SCANCODE_S), 
      _reload(SDL_SCANCODE_SPACE), _collect(SDL_SCANCODE_F), 
      _mc(nullptr), _w(nullptr), _dc(nullptr) {}

KeyboardPlayerCtrl::~KeyboardPlayerCtrl() {}

void 
KeyboardPlayerCtrl::initComponent() {

    /*
    _tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
    assert(_tr != nullptr);
    */
    _mc = Game::Instance()->get_mngr()->getComponent<MovementController>(_ent);
    assert(_mc != nullptr);

    _w = Game::Instance()->get_mngr()->getComponent<Weapon>(_ent);
    assert(_w != nullptr);

    _dc = Game::Instance()->get_mngr()->getComponent<Deck>(_ent);
    assert(_dc != nullptr);
}

void KeyboardPlayerCtrl::update(Uint32 delta_time) {
    (void)delta_time;
    auto& ihdlr = ih();
    //auto& dir = _tr->getDir();
    //Horizontal axis
    _mc->set_input(Vector2D(
        (ihdlr.isKeyDown(_left) ? -1 : 0) + (ihdlr.isKeyDown(_right) ? 1 : 0),
        (ihdlr.isKeyDown(_up) ? 1 : 0) + (ihdlr.isKeyDown(_down) ? -1 : 0)
    ));

    //Vertical axis
    //dir.setY((ihdlr.isKeyDown(_up) ? -1 : 0) + (ihdlr.isKeyDown(_down) ? 1 : 0));

    //reload
    if (ihdlr.isKeyDown(_reload)) {
        //std::cout << "recarga" << std::endl;
        _dc->reload();
    }

    //collect
    if (ihdlr.isKeyDown(_collect)) {
        //if we are not close enought to a reward, do nothing
        std::cout << "colecta" << std::endl;
    }


    if (ihdlr.mouseButtonDownEvent()) {
        rect_f32 converted_mouse_pos = rect_f32_global_from_screen_rect_flipped_y(
            {
                {
                    ih().getMousePos().getX(),
                    ih().getMousePos().getY()
                },
                {
                    1,1
                }
            },
            Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->cam
        );
        //Vector2D mousePos = { (float)ihdlr.getMousePos().getX(), (float)ihdlr.getMousePos().getY()};
        //shoot
        if (ihdlr.getMouseButtonState(InputHandler::LEFT)) {
            //send message to shootih().getMousePos();
            //_w->shoot(mousePos);
            if (_dc->discard_card()) {
                //position2_f32 mouse_pos = Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->mouse_world_position; 
                _w->shoot(Vector2D{ converted_mouse_pos.position.x, converted_mouse_pos.position.y });
            }       
        }
        //use card
        else if (ihdlr.getMouseButtonState(InputHandler::RIGHT)) {
            //send message to use a card
            Vector2D* vec = new Vector2D{ converted_mouse_pos.position.x,  converted_mouse_pos.position.y };
            _dc->use_card(vec);
        }
    }
    
}