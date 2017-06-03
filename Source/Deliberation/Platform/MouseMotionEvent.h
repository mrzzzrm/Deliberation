#pragma once

#include <Deliberation/Platform/MouseStateEvent.h>

namespace deliberation
{
class MouseMotionEvent : public MouseStateEvent
{
  public:
    MouseMotionEvent(
        u32 buttons,
        const glm::vec2 & mousePosition,
        const glm::vec2 & mouseMovement):
        MouseStateEvent(buttons, mousePosition)
        , m_mouseMovement(mouseMovement)
    {
    }

    const glm::vec2 & mouseMovement() const { return m_mouseMovement; }

  private:
    glm::vec2   m_mouseMovement;
};
}