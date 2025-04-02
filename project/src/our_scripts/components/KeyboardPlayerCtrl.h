#pragma once
#include "../../ecs/Component.h" 
#include <SDL.h>

class Transform;
class Weapon;
class Deck;
class MovementController;
class KeyboardPlayerCtrl :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::KEYBOARDPLAYERCTRL);
	KeyboardPlayerCtrl(); 
	virtual ~KeyboardPlayerCtrl(); 
	void initComponent() override;
	void update(Uint32 delta_time) override;
private:
	SDL_Scancode _left;
	SDL_Scancode _right;
	SDL_Scancode _up;
	SDL_Scancode _down;
	SDL_Scancode _reload;
	SDL_Scancode _collect;
	Vector2D _mouse_pos;
	MovementController* _mc;
	Weapon* _w;
	Deck* _dc;
};