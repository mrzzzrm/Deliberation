#include <Deliberation/Platform/Input.h>

#include <algorithm>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Platform/KeyMap.h>
#include <Deliberation/Platform/InputLayer.h>

namespace
{

using namespace deliberation;

deliberation::Key SDLKeyToDeliberation(SDL_Keycode code)
{
    if (code >= SDLK_a && code <= SDLK_z)
    {
        return (Key)(Key_A + (code - SDLK_a));
    }

    switch(code) {
        case SDLK_RIGHT: return Key_RIGHT;
        case SDLK_LEFT: return Key_LEFT;
        case SDLK_UP: return Key_UP;
        case SDLK_DOWN: return Key_DOWN;
        case SDLK_SPACE: return Key_SPACE;
    }

    return deliberation::Key_UNKNOWN;
}

    deliberation::MouseButton SDLMouseButtonToDeliberation(uint8_t button)
{
    switch (button)
    {
        case SDL_BUTTON_LEFT:
            return deliberation::MouseButton_Left;
        case SDL_BUTTON_RIGHT:
            return deliberation::MouseButton_Right;
    }

    return deliberation::MouseButton_Unknown;
}

}

namespace deliberation
{

Input::Input(const DrawContext & drawContext):
    m_drawContext(drawContext)
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
            glm::vec2 canvasSize(m_drawContext.backbuffer().width(), m_drawContext.backbuffer().height());
            auto normalized = glm::vec2(event.motion.x, event.motion.y) / canvasSize * 2.0f - 1.0f;
            normalized.y *= -1.0f;

            setMousePosition(normalized);
            break;
    }
}

}
