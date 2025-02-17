#pragma once
#include "../../ecs/Component.h" 
#include <SDL.h>

class Transform;
class ShootComponent;
class KeyboardPlayerCtrl :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::KEYBOARDPLAYERCTRL);
	KeyboardPlayerCtrl(); 
	virtual ~KeyboardPlayerCtrl(); 
	void init_component() override;
	void update(int delta_time) override;
private:
	SDL_Scancode _left;
	SDL_Scancode _right;
	SDL_Scancode _up;
	SDL_Scancode _down;
	SDL_Scancode _reload;
	SDL_Scancode _collect;

	Transform* _tr;
	ShootComponent* _sc;
};