#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Platform/InputEvent.h>
#include <Deliberation/Platform/MouseButton.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class MouseMotionEvent final : public InputEvent
{
  public:
    MouseMotionEvent(
        MouseButton       buttons,
        const glm::vec2 & mousePosition,
        const glm::vec2 & mouseMovement)
        : m_buttons(buttons)
        , m_mousePosition(mousePosition)
        , m_mouseMovement(mouseMovement)
    {
    }

    bool button(MouseButton button) const { return m_buttons & button; }
    const glm::vec2 & mousePosition() const { return m_mousePosition; }
    const glm::vec2 & mouseMovement() const { return m_mouseMovement; }

  private:
    MouseButton m_buttons;
    glm::vec2   m_mousePosition;
    glm::vec2   m_mouseMovement;
};
}