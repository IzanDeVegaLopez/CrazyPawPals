#pragma once

#include "../../utils/Vector2D.h"
#include "../../game/GameObject.h"
#include <functional>

class Transform;

//Usa Callbacks a funciones de tipo <void(void)>
using SDLEventCallback = std::function<void(void)>;

class Button : public GameObject
{
public:
	Button();
	~Button();

	void initComponent() override;
	void update() override;

	int getButtonState() { return _currentState; }

	//Clicks and actions
	void actionsOfButton(SDLEventCallback _callback);
	void leftClickUp();
	void leftClickDown();
	bool mouseOver(SDL_Rect* collider);

private:
	//Pressing states
	enum ButtonState { EMPTY, HOVER, CLICK };
	int _currentState;

	//Callbacks
	std::list<SDLEventCallback> callbacks;
	void callback() const;

	SDL_Rect buttonCollider;
	Transform* _tr;
};