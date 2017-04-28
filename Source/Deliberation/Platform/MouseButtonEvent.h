#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Platform/MouseButton.h>
#include <Deliberation/Platform/InputEvent.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class MouseButtonEvent final:
    public InputEvent
{
public:
    MouseButtonEvent(MouseButton button, const glm::vec2 & mousePosition):
        m_button(button), m_mousePosition(mousePosition)
    {}

    MouseButton button() const { return m_button; }
    const glm::vec2 & mousePosition() const { return m_mousePosition; }

private:
    MouseButton  m_button;
    glm::vec2    m_mousePosition;
};

}