#include <Deliberation/Platform/Input.h>

#include <Deliberation/Draw/Context.h>

namespace
{

using namespace deliberation;

deliberation::InputBase::Key SDLKeyToDeliberation(SDL_Keycode code)
{
    if (code >= SDLK_a && code <= SDLK_z)
    {
        return (InputBase::Key)(InputBase::Key_A + (code - SDLK_a));
    }

    switch(code) {
        case SDLK_RIGHT: return InputBase::Key_RIGHT;
        case SDLK_LEFT: return InputBase::Key_LEFT;
        case SDLK_UP: return InputBase::Key_UP;
        case SDLK_DOWN: return InputBase::Key_DOWN;
        case SDLK_SPACE: return InputBase::Key_SPACE;
    }

    return deliberation::InputBase::Key_UNKNOWN;
}

    deliberation::InputBase::MouseButton SDLMouseButtonToDeliberation(uint8_t button)
{
    switch (button)
    {
        case SDL_BUTTON_LEFT:
            return deliberation::InputBase::MouseButton_Left;
        case SDL_BUTTON_RIGHT:
            return deliberation::InputBase::MouseButton_Right;
    }

    return deliberation::InputBase::MouseButton_Unknown;
}

}

namespace deliberation
{

Input::Input(const Context & context):
    m_context(context)
{

}

void Input::onSDLInputEvent(const SDL_Event & event)
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
            glm::vec2 canvasSize(m_context.backbuffer().width(), m_context.backbuffer().height());
            auto normalized = glm::vec2(event.motion.x, event.motion.y) / canvasSize * 2.0f - 1.0f;
            normalized.y *= -1.0f;

            setMousePosition(normalized);
            break;
    }

}

}
