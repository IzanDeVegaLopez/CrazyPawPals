#pragma once
#include "../../ecs/Component.h" 
#include <SDL.h>
class GamePadPlayerCtrl :public ecs::Component
{
private:
    MovementController* _mc;
    Weapon* _w;
    Deck* _dc;
    Health* _h;
    ManaComponent* _m;
    Transform* _tr;
    AnimationComponent* _dy;
    Vector2D _reticle_position;
public:
	__CMPID_DECL__(ecs::cmp::GAMEPAD_PLAYER_CTRL);
	inline GamePadPlayerCtrl() {
        /*
		for (int i = 0; i < SDL_NumJoysticks(); ++i) {
			std::cout << SDL_JoystickNameForIndex(i);
		}
        */
	}
    inline void initComponent() {
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

        _h = Game::Instance()->get_mngr()->getComponent<Health>(_ent);
        assert(_h != nullptr);

        _m = Game::Instance()->get_mngr()->getComponent<ManaComponent>(_ent);
        assert(_m != nullptr);

        _tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
        assert(_tr != nullptr);

        _dy = Game::Instance()->get_mngr()->getComponent<AnimationComponent>(_ent);
        assert(_dy != nullptr);
    }

    inline void update(Uint32 delta_time) override {
        if (ih().getLastDevice() != InputHandler::LAST_DEVICE_ACTIVE::CONTROLLER) return;

        _reticle_position = _tr->getPos() + (ih().getRStick()*300);
        _mc->set_input(ih().getLStick());
        if (ih().getLStick().magnitude()>0.2) _dy->play_animation("andar");
        else _dy->play_animation("idle");
        if (ih().isControllerButtonDown(InputHandler::CONTROLLER_BUTTONS::LT)) {
            _dc->use_card(&_reticle_position);
        }
        if (ih().isControllerButtonDown(InputHandler::CONTROLLER_BUTTONS::RT)) {
            auto dir = ih().getRStick();
            if (!_dc->empty_hand() && _w->shoot(dir.getX(), dir.getY())) {
                //position2_f32 mouse_pos = Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA))->mouse_world_position; 
                _dc->discard_card();
            }
        }
        if (ih().isControllerButtonDown(InputHandler::CONTROLLER_BUTTONS::B)) {
            _dc->reload();
        }
    }

};