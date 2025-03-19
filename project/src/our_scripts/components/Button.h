#pragma once
#include "../../utils/Vector2D.h"
#include "../../ecs/Component.h"
#include <functional>
#include <list>
#include "../../sdlutils/SDLUtils.h"

using SDLEventCallback = std::function<void(void)>;

class Button : public ecs::Component {
public:
    __CMPID_DECL__(ecs::cmp::BUTTON);
    Button();
    ~Button();

    void initComponent() override;
    void update(uint32_t delta_time) override;

    // clic and hover events
    void leftClickUp();
    void leftClickDown();
    bool mouseOver();

    // link methods
    void connectClick(SDLEventCallback callback);
    void connectHover(SDLEventCallback callback);
    void connectExit(SDLEventCallback callback);
private:
    // Button states
    enum ButtonState { EMPTY, HOVER, CLICK };
    ButtonState _current_state;
    ButtonState _previous_state;

    // Callbacks
    std::list<SDLEventCallback> _click_callbacks;  
    std::list<SDLEventCallback> _hover_callbacks;  
    std::list<SDLEventCallback> _pointer_exit_callbacks;

    void emitClick() const;
    void emitHover() const; 
    void emitExit() const;

    SDL_Rect _button_collider;
};