#include <Deliberation/Core/InputBase.h>

#include <algorithm>
#include <iostream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Platform/InputLayer.h>
#include <Deliberation/Core/Chrono.h>

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
    if (button >= m_mouseButtonsPressedTimestamps.size())
    {
        return false;
    }
    return m_mouseButtonsPressedTimestamps[button] != INVALID_TIMESTAMP_MILLIS;
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

    if (button >= m_mouseButtonsPressedTimestamps.size())
    {
        m_mouseButtonsPressedTimestamps.resize(button + 1, INVALID_TIMESTAMP_MILLIS);
    }

    if (down)
    {
        if (m_mouseButtonsPressedTimestamps[button] != INVALID_TIMESTAMP_MILLIS && !mouseButtonPressed(button))
        {
            m_pressedMouseButtons.push_back(button);
            m_mouseButtonPressedEvents.emplace_back((MouseButton)button, m_mousePosition);
        }
    }

    if (down)
    {
        m_mouseButtonsPressedTimestamps[button] = CurrentMillis();
    }
    else
    {
        if (m_mouseButtonsPressedTimestamps[button] != INVALID_TIMESTAMP_MILLIS)
        {
            const auto downDuration = CurrentMillis() - m_mouseButtonsPressedTimestamps[button];
            if (downDuration < CLICK_TIMEOUT)
            {
                m_mouseButtonClickedEvents.emplace_back((MouseButton)button, m_mousePosition);
            }
        }

        m_mouseButtonsPressedTimestamps[button] = INVALID_TIMESTAMP_MILLIS;
    }
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

void InputBase::addLayer(const std::shared_ptr<InputLayer> & layer)
{
    m_layers.emplace_back(layer);

    std::sort(m_layers.begin(), m_layers.end(),
              [] (const std::shared_ptr<InputLayer> & a, const std::shared_ptr<InputLayer> & b)
              {
                  return a->inputPriority() > b->inputPriority();
              });
}

void InputBase::removeLayer(const std::shared_ptr<InputLayer> & layer)
{
    const auto iter = std::find(m_layers.begin(), m_layers.end(), layer);
    Assert(iter != m_layers.end(), "");

    m_layers.erase(iter);
}

void InputBase::processInput()
{
    processEventQueue(m_mouseButtonPressedEvents,
                      [] (InputLayer & layer, MouseButtonEvent & event) { layer.onMouseButtonPressed(event); });

    for (size_t b = 0; b < m_mouseButtonsPressedTimestamps.size(); b++)
    {
        if (m_mouseButtonsPressedTimestamps[b] != INVALID_TIMESTAMP_MILLIS)
        {
            m_mouseButtonDownEvents.emplace_back((MouseButton)b, m_mousePosition);
        }
    }

    processEventQueue(m_mouseButtonDownEvents,
                      [] (InputLayer & layer, MouseButtonEvent & event) { layer.onMouseButtonDown(event); });

    processEventQueue(m_mouseButtonClickedEvents,
                      [] (InputLayer & layer, MouseButtonEvent & event) { layer.onMouseButtonClicked(event); });

}

template<typename T>
void InputBase::processEventQueue(T & queue,
                                  const std::function<void(InputLayer &, typename T::value_type &)> & fn) const
{
    for (auto & event : queue)
    {
        for (const auto & layer : m_layers)
        {
            fn(*layer, event);
            if (event.isConsumed()) break;
        }
    }
    queue.clear();
}

}

