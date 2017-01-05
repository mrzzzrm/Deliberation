#include <Deliberation/Core/InputBase.h>

#include <algorithm>
#include <iostream>

namespace deliberation
{

InputBase::InputBase() = default;

InputBase::~InputBase() = default;

bool InputBase::mouseButtonPressed(unsigned int button) const
{
    auto it = std::find(m_pressedMouseButtons.begin(), m_pressedMouseButtons.end(), button);
    return it != m_pressedMouseButtons.end();
}

bool InputBase::keyPressed(unsigned int key) const
{
    auto it = std::find(m_pressedKeys.begin(), m_pressedKeys.end(), key);
    return it != m_pressedKeys.end();
}

bool InputBase::mouseButtonDown(unsigned int button) const
{
    if (button >= m_mouseButtons.size())
    {
        return false;
    }
    return m_mouseButtons[button];
}

bool InputBase::keyDown(unsigned int key) const
{
    if (key >= m_keys.size())
    {
        return false;
    }
    return m_keys[key];
}

const glm::vec2 & InputBase::mousePosition() const
{
    return m_mousePosition;
}

void InputBase::setMouseButtonDown(unsigned int button, bool down)
{
    static bool haveWarned = false;
    if (button > 4096 && !haveWarned)
    {
        std::cout << "Consider switching container, button index is " << button << std::endl;
        haveWarned = true;
    }

    if (button >= m_mouseButtons.size())
    {
        m_mouseButtons.resize(button + 1, false);
    }

    if (down)
    {
        if (!m_mouseButtons[button] && !mouseButtonPressed(button))
        {
            m_pressedMouseButtons.push_back(button);
        }
    }

    m_mouseButtons[button] = down;
}

void InputBase::setKeyDown(unsigned int key, bool down)
{
    static bool haveWarned = false;
    if (key > 4096 && !haveWarned)
    {
        std::cout << "Consider switching container, key index is " << key << std::endl;
        haveWarned = true;
    }

    if (key >= m_keys.size())
    {
        m_keys.resize(key + 1, false);
    }

    if (down)
    {
        if (!m_keys[key] && !keyPressed(key))
        {
            m_pressedKeys.push_back(key);
        }
    }

    m_keys[key] = down;
}

void InputBase::setMousePosition(const glm::vec2 & position)
{
    m_mousePosition = position;
}

void InputBase::step()
{
    m_pressedMouseButtons.clear();
    m_pressedKeys.clear();
}

}

