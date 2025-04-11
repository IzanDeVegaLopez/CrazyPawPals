#pragma once
#include "../../ecs/Component.h" 
#include <SDL.h>

class Transform;
class Health;
class Weapon;
class Deck;
class ManaComponent;
class MovementController;
class MythicComponent;
class AnimationComponent;

class KeyboardPlayerCtrl :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::KEYBOARDPLAYERCTRL);
	KeyboardPlayerCtrl(); 
	virtual ~KeyboardPlayerCtrl(); 
	void initComponent() override;
	void update(Uint32 delta_time) override;

	bool is_moving_input() const;
	inline Vector2D get_reticle_position() {
		return _mouse_pos;
	}
private:
	SDL_Scancode _left;
	SDL_Scancode _right;
	SDL_Scancode _up;
	SDL_Scancode _down;
	SDL_Scancode _reload;
	SDL_Scancode _collect;
	Vector2D _mouse_pos;

	AnimationComponent* _dy;
	MovementController* _mc;
	Weapon* _w;
	Deck* _dc;
	MythicComponent* _my;
	Health* _h;
	ManaComponent* _m;
	Transform* _tr;
};