#include <Deliberation/Platform/InputAdapter.h>

namespace
{

deliberation::InputAdapterBase::Key SDLKeyToDeliberation(SDL_Keycode code)
{
    if (code >= SDLK_a && code <= SDLK_z)
    {
        return (deliberation::InputAdapterBase::Key)(deliberation::InputAdapterBase::Key_A + (code - SDLK_a));
    }

    return deliberation::InputAdapterBase::Key_UNKNOWN;
}

    deliberation::InputAdapterBase::MouseButton SDLMouseButtonToDeliberation(uint8_t button)
{
    switch (button)
    {
        case SDL_BUTTON_LEFT:
            return deliberation::InputAdapterBase::MouseButton_Left;
        case SDL_BUTTON_RIGHT:
            return deliberation::InputAdapterBase::MouseButton_Right;
    }

    return deliberation::InputAdapterBase::MouseButton_Unknown;
}

}

namespace deliberation
{

void InputAdapter::onSDLInputEvent(const SDL_Event & event)
{
    switch (event.type)
    {
        case SDL_KEYUP:
            setKeyDown(SDLKeyToDeliberation(event.key.keysym.sym), false);
            break;

        case SDL_KEYDOWN:
            setKeyDown(SDLKeyToDeliberation(event.key.keysym.sym), true);
            break;

        case SDL_MOUSEBUTTONUP:
            setMouseButtonDown(SDLMouseButtonToDeliberation(event.button.button), false);
            break;

        case SDL_MOUSEBUTTONDOWN:
            setMouseButtonDown(SDLMouseButtonToDeliberation(event.button.button), true);
            break;

        case SDL_MOUSEMOTION:
            setMousePosition({event.motion.x, event.motion.y});
            break;
    }

}

}
