#pragma once
#include "InputComponent.h"

#include <SDL.h>

class KeyboardPlayerCtrl :public InputComponent
{
public:
	KeyboardPlayerCtrl(); 
	virtual ~KeyboardPlayerCtrl(); 
	void handleInput(Container* o) override;
private:
	SDL_Scancode _left;
	SDL_Scancode _right;
	SDL_Scancode _up;
	SDL_Scancode _down;
	SDL_Scancode _reload;
	SDL_Scancode _collect;
};