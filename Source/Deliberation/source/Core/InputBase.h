#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Platform/MouseButtonEvent.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class InputLayer;

class InputBase
{
public:
    static constexpr DurationMillis CLICK_TIMEOUT = 300;

public:
    InputBase();
    virtual ~InputBase();

    bool mouseButtonPressed(unsigned int button) const;
    bool keyPressed(unsigned int key) const;

    bool mouseButtonDown(unsigned int button) const;
    bool keyDown(unsigned int key) const;
    const glm::vec2 & mousePosition() const;

    void setMouseButtonDown(unsigned int button, bool down);
    void setKeyDown(unsigned int key, bool down);
    void setMousePosition(const glm::vec2 & position);

    void step();

    void addLayer(const std::shared_ptr<InputLayer> & layer);
    void removeLayer(const std::shared_ptr<InputLayer> & layer);

    void processInput();

private:
    template<typename T>
    void processEventQueue(T & queue, const std::function<void(InputLayer &, typename T::value_type &)> & fn) const;

private:
    std::vector<TimestampMillis>    m_mouseButtonsPressedTimestamps;
    std::vector<uint>               m_pressedMouseButtons;

    std::vector<bool>               m_keys;
    std::vector<uint>               m_pressedKeys;

    glm::vec2                       m_mousePosition;

    std::vector<MouseButtonEvent>   m_mouseButtonPressedEvents;
    std::vector<MouseButtonEvent>   m_mouseButtonDownEvents;
    std::vector<MouseButtonEvent>   m_mouseButtonClickedEvents;

    std::vector<std::shared_ptr<InputLayer>>
                                    m_layers;
};

}

