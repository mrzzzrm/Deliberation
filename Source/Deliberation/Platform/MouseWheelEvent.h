#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Platform/InputEvent.h>
#include <Deliberation/Platform/MouseButton.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class MouseWheelEvent final : public InputEvent
{
public:
    MouseWheelEvent(float x, float y) : m_x(x), m_y(y) {}

    float x() const { return m_x; }
    float y() const { return m_y; }

private:
    float m_x;
    float m_y;
};
}