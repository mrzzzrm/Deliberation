#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Platform/InputEvent.h>
#include <Deliberation/Platform/MouseButton.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class MouseStateEvent : public InputEvent
{
public:
    MouseStateEvent(
        u32 buttons,
        const glm::vec2 & mousePosition)
        : m_buttons(buttons)
        , m_mousePosition(mousePosition)
    {
    }

    bool button(MouseButton button) const { return m_buttons & (1 << (i32)button); }
    const glm::vec2 & mousePosition() const { return m_mousePosition; }

private:
    u32         m_buttons;
    glm::vec2   m_mousePosition;
};
}