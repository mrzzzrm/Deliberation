#include <Deliberation/Platform/Input.h>

#include <algorithm>
#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Platform/InputLayer.h>
#include <Deliberation/Platform/KeyEvent.h>
#include <Deliberation/Platform/KeyMap.h>
#include <Deliberation/Platform/MouseMotionEvent.h>
#include <Deliberation/Platform/MouseWheelEvent.h>

namespace
{
using namespace deliberation;

Key SDLKeyToDeliberation(SDL_Keycode code)
{
    if (code >= SDLK_a && code <= SDLK_z)
    {
        return (Key)(Key_A + (code - SDLK_a));
    }

    switch (code)
    {
    case SDLK_RIGHT: return Key_RIGHT;
    case SDLK_LEFT: return Key_LEFT;
    case SDLK_UP: return Key_UP;
    case SDLK_DOWN: return Key_DOWN;
    case SDLK_SPACE: return Key_SPACE;
    }

    return deliberation::Key_UNKNOWN;
}

u32 DeliberationKeyToSDLKeycode(Key key)
{
    if (key >= Key_A && key <= Key_Z)
    {
        return SDLK_a + (key - Key_A);
    }

    switch (key)
    {
    case Key_RIGHT: return SDLK_RIGHT;
    case Key_LEFT: return SDLK_LEFT;
    case Key_UP: return SDLK_UP;
    case Key_DOWN: return SDLK_DOWN;
    case Key_SPACE: return SDLK_SPACE;
    }

    return SDLK_UNKNOWN;
}
u32 DeliberationKeyToSDLScancode(Key key)
{
    if (key >= Key_A && key <= Key_Z)
    {
        return SDL_SCANCODE_A + (key - Key_A);
    }

    switch (key)
    {
    case Key_RIGHT: return SDL_SCANCODE_RIGHT;
    case Key_LEFT: return SDLK_LEFT;
    case Key_UP: return SDLK_UP;
    case Key_DOWN: return SDLK_DOWN;
    case Key_SPACE: return SDLK_SPACE;
    }

    return SDLK_UNKNOWN;
}

MouseButton SDLMouseButtonToDeliberation(uint8_t button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT: return deliberation::MouseButton::Left;
    case SDL_BUTTON_RIGHT: return deliberation::MouseButton::Right;
    case SDL_BUTTON_MIDDLE: return deliberation::MouseButton::Middle;
    case SDL_BUTTON_X1: return deliberation::MouseButton::X1;
    case SDL_BUTTON_X2: return deliberation::MouseButton::X2;
    default: return deliberation::MouseButton::Unknown;
    }
}

u32 DeliberationMouseButtonToSDL(MouseButton mouseButton)
{
    switch (mouseButton)
    {
    case MouseButton::Left: return SDL_BUTTON_LEFT;
    case MouseButton::Middle: return SDL_BUTTON_MIDDLE;
    case MouseButton::Right: return SDL_BUTTON_RIGHT;
    case MouseButton::X1: return SDL_BUTTON_X1;
    case MouseButton::X2: return SDL_BUTTON_X2;
    default: Fail("");
    }
}
}

namespace deliberation
{
Input::Input() { m_mouseButtonsDown.reset(); }

bool Input::mouseButtonDown(MouseButton button) const
{
    return (SDL_GetMouseState(NULL, NULL) &
            SDL_BUTTON(DeliberationMouseButtonToSDL(button))) != 0;
}

bool Input::keyPressed(Key key) const
{
    return SDL_GetKeyboardState(NULL)[DeliberationKeyToSDLScancode(key)];
}

glm::vec2 Input::mousePosition() const
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return sdlMousePositionToNdc(x, y);
}

void Input::addLayer(const std::shared_ptr<InputLayer> & layer)
{
    m_layers.emplace_back(layer);

    std::sort(
        m_layers.begin(),
        m_layers.end(),
        [](const std::shared_ptr<InputLayer> & a,
           const std::shared_ptr<InputLayer> & b) {
            return a->inputPriority() > b->inputPriority();
        });
}

void Input::removeLayer(const std::shared_ptr<InputLayer> & layer)
{
    const auto iter = std::find(m_layers.begin(), m_layers.end(), layer);
    Assert(iter != m_layers.end(), "");

    m_layers.erase(iter);
}

void Input::onSDLInputEvent(const SDL_Event & sdlEvent)
{
    switch (sdlEvent.type)
    {
    case SDL_KEYUP:
        processEvent<KeyEvent>(
            KeyEvent(SDLKeyToDeliberation(sdlEvent.key.keysym.sym)),
            [&](InputLayer & layer, KeyEvent & event) {
                layer.onKeyReleased(event);
            });
        break;

    case SDL_KEYDOWN:
        processEvent<KeyEvent>(
            KeyEvent(SDLKeyToDeliberation(sdlEvent.key.keysym.sym)),
            [&](InputLayer & layer, KeyEvent & event) {
                layer.onKeyPressed(event);
            });
        break;

    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
    {
        const auto mouseButton =
            SDLMouseButtonToDeliberation(sdlEvent.button.button);
        const auto event = MouseButtonEvent(
            mouseButton,
            sdlMousePositionToNdc(sdlEvent.button.x, sdlEvent.button.y));
        // Detect and dispatch clicks
        if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
        {
            m_downTimestampByMouseButton[mouseButton] =
                sdlEvent.button.timestamp;
        }
        else
        {
            if (sdlEvent.button.timestamp -
                    m_downTimestampByMouseButton[mouseButton] <=
                CLICK_TIMEOUT)
            {
                processEvent<MouseButtonEvent>(
                    event, [&](InputLayer & layer, MouseButtonEvent & event) {
                        layer.onMouseButtonClicked(event);
                    });
            }
        }

        // Dispatch Down/Release
        auto receivingLayer = processEvent<MouseButtonEvent>(
            event, [&](InputLayer & layer, MouseButtonEvent & event) {
                if (sdlEvent.type == SDL_MOUSEBUTTONUP)
                    layer.onMouseButtonReleased(event);
                if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
                    layer.onMouseButtonPressed(event);
            });

        if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
            m_mouseOwningLayer = receivingLayer;

        // Mouse Button Down state/events
        if (sdlEvent.type == SDL_MOUSEBUTTONUP)
            m_mouseButtonsDown.reset((size_t)mouseButton);
        else
            m_mouseButtonsDown.set((size_t)mouseButton);

        if (sdlEvent.type == SDL_MOUSEBUTTONUP &&
            m_mouseButtonsDown.count() == 0)
            m_mouseOwningLayer.reset();
    }
    break;

    case SDL_MOUSEMOTION:
    {
        auto mouseButtons = 0;
        mouseButtons =
            (mouseButtons | (sdlEvent.motion.state & SDL_BUTTON_LMASK
                                 ? (i32)MouseButtonMask::Left
                                 : 0));
        mouseButtons =
            (mouseButtons | (sdlEvent.motion.state & SDL_BUTTON_MMASK
                                 ? (i32)MouseButtonMask::Middle
                                 : 0));
        mouseButtons =
            (mouseButtons | (sdlEvent.motion.state & SDL_BUTTON_RMASK
                                 ? (i32)MouseButtonMask::Right
                                 : 0));
        mouseButtons =
            (mouseButtons | (sdlEvent.motion.state & SDL_BUTTON_X1MASK
                                 ? (i32)MouseButtonMask::X1
                                 : 0));
        mouseButtons =
            (mouseButtons | (sdlEvent.motion.state & SDL_BUTTON_X2MASK
                                 ? (i32)MouseButtonMask::X2
                                 : 0));

        const auto event = MouseMotionEvent(
            mouseButtons,
            sdlMousePositionToNdc(sdlEvent.motion.x, sdlEvent.motion.y),
            sdlMousePositionToNdc(sdlEvent.motion.xrel, sdlEvent.motion.yrel) -
                glm::vec2(-1.0f, 1.0f));

        processEvent<MouseMotionEvent>(
            event, [&](InputLayer & layer, MouseMotionEvent & event) {
                layer.onMouseMotion(event);
            });
    }
    break;

    case SDL_MOUSEWHEEL:
    {
        processEvent<MouseWheelEvent>(
            MouseWheelEvent(sdlEvent.wheel.x, sdlEvent.wheel.y),
            [&](InputLayer & layer, MouseWheelEvent & event) {
                layer.onMouseWheel(event);
            });
    }
    break;
    }
}

void Input::onFrameBegin()
{
    // MouseDown event
    {
        int mouseX;
        int mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        MouseStateEvent event =
            MouseStateEvent{(u32)m_mouseButtonsDown.to_ulong(),
                            sdlMousePositionToNdc(mouseX, mouseY)};

        if (m_mouseOwningLayer)
        {
            m_mouseOwningLayer->onMouseButtonDown(event);
        }
        else
        {
            processEvent<MouseStateEvent>(
                event, [&](InputLayer & layer, MouseStateEvent & event) {
                    layer.onMouseButtonDown(event);
                });
        }
    }
}

template<typename T>
std::shared_ptr<InputLayer> Input::processEvent(
    T event, const std::function<void(InputLayer &, T &)> & fn) const
{
    for (const auto & layer : m_layers)
    {
        fn(*layer, event);
        if (event.isConsumed()) return layer;
    }
    return {};
}

glm::vec2 Input::sdlMousePositionToNdc(i32 x, i32 y) const
{
    // This might be the wrong window, but duuh, not supporting multiple atm
    // anyway
    auto * sdlWindow = SDL_GL_GetCurrentWindow();

    int w, h;
    SDL_GetWindowSize(sdlWindow, &w, &h);

    glm::vec2 canvasSize(w, h);
    auto      normalized = glm::vec2(x, y) / canvasSize * 2.0f - 1.0f;
    normalized.y *= -1.0f;

    return normalized;
}
}
