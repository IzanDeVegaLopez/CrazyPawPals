#include "KeyboardPlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../components/rendering/camera_component.hpp"

#include "cards/Deck.hpp"
#include "movement/Transform.h"
#include "weapons/Weapon.h"
#include "movement/MovementController.h"
#include "Health.h"
#include "MythicComponent.h"
#include "../mythic/MythicItems.h"

KeyboardPlayerCtrl::KeyboardPlayerCtrl()
    : _left(SDL_SCANCODE_A), _right(SDL_SCANCODE_D), _up(SDL_SCANCODE_W), _down(SDL_SCANCODE_S), 
      _reload(SDL_SCANCODE_SPACE), _collect(SDL_SCANCODE_F), 
      _mc(nullptr), _w(nullptr), _dc(nullptr), _mouse_pos(Vector2D(0,0)) {}

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

    _my = Game::Instance()->get_mngr()->getComponent<MythicComponent>(_ent);
    assert(_my != nullptr);

    _h = Game::Instance()->get_mngr()->getComponent<Health>(_ent);
    assert(_h != nullptr);

    _m = Game::Instance()->get_mngr()->getComponent<ManaComponent>(_ent);
    assert(_m != nullptr);
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
        //std::cout << "colecta" << std::endl;
    }

    ///inputs para probar cosas
    if (ihdlr.keyDownEvent() &&ihdlr.isKeyDown(SDL_SCANCODE_Y)) {
        _my->add_mythic(new ArcaneSurge(_m, _dc));
    }  
      if (ihdlr.keyDownEvent() &&ihdlr.isKeyDown(SDL_SCANCODE_G)) {
        _my->add_mythic(new PreternaturalForce(_m, _w));
    } 
    if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_Z)) {
        _my->add_mythic(new BloodClaw(_h,_w));
    }
    if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_X)) {
        _my->add_mythic(new ProfaneHotline(_m));
    }

    if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_V)) {
        _h->takeDamage(10);
    }
    ////

    auto _mouse_rect =rect_f32_global_from_screen_rect_flipped_y(
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
    _mouse_pos = Vector2D(_mouse_rect.position.x, _mouse_rect.position.y);


    if (ihdlr.mouseButtonDownEvent()) {

        //Vector2D mousePos = { (float)ihdlr.getMousePos().getX(), (float)ihdlr.getMousePos().getY()};
        //shoot
        if (ihdlr.getMouseButtonState(InputHandler::LEFT)) {
            //send message to shootih().getMousePos();
            //_w->shoot(mousePos);
            if (!_dc->empty_hand() && _w->shoot(_mouse_pos)) {
                //position2_f32 mouse_pos = Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->mouse_world_position; 
                _dc->discard_card();
            }       
        }
        //use card
        else if (ihdlr.getMouseButtonState(InputHandler::RIGHT)) {
            //send message to use a card
            //Vector2D* vec = new Vector2D{ converted_mouse_pos.position.x,  converted_mouse_pos.position.y };
            _dc->use_card(&_mouse_pos);
        }
    }
    
}