//#include "Button.h"
//
//#include "../../sdlutils/SDLUtils.h"
//#include "../../sdlutils/InputHandler.h"
//#include "../../ecs/Manager.h"
//#include "Transform.h"
//#include <iostream>
//
//using namespace ecs;
//
//Button::Button()
//{
//	//Is not being pressed or hovered
//	_currentState = EMPTY;
//
//	//InputHandler
//	//ih().insertFunction(ih().MOUSE_LEFT_CLICK_DOWN, [this] { leftClickDown(); });
//	//ih().insertFunction(ih().MOUSE_LEFT_CLICK_UP, [this] { leftClickUp(); });
//}
//
//Button::~Button()
//{
//	//ih().clearFunction(InputHandler::MOUSE_LEFT_CLICK_DOWN, [this] {leftClickDown();});
//	//ih().clearFunction(InputHandler::MOUSE_LEFT_CLICK_UP, [this] {leftClickUp();});
//}
//
//void Button::initComponent()
//{
//	auto* mngr = _ent->getMngr();
//	_tr = mngr->getComponent<Transform>(_ent);
//	assert(_tr != nullptr);
//
//	buttonCollider = new SDL_Rect{ _tr->getPos().getX(), _tr->getPos().getY(), _tr->getWidth(), _tr->getHeight() };
//}
//
//void Button::update()
//{
//	// If mouse over button, hover
//	if (mouseOver(buttonCollider)) {
//		_currentState = HOVER;
//	}
//	else
//	{
//		_currentState = EMPTY; 
//	}
//}
//
//void Button::actionsOfButton(SDLEvent
// 
// 
//  _callback)
//{
//	callbacks.push_back(_callback);
//}
//
//void Button::leftClickUp()
//{
//	_currentState = EMPTY;
//}
//
//void Button::leftClickDown()
//{
//	if (mouseOver()) && _currentState == HOVER) {
//		_currentState = CLICK;
//		callback();
//	}
//}
//
//bool Button::mouseOver(SDL_Rect* collider)
//{
//	Vector2D mousePos(ih().getMousePos().first, ih().getMousePos().second);
//
//	SDL_Rect mouseRect = build_sdlrect(mousePos, 1, 1);
//
//	return SDL_HasIntersection(&collider, &mouseRect);
//}
//
//
////Private method
//void Button::callback() const
//{
//	for (auto c : callbacks) { c(); }
//}
