#pragma once

#include <Deliberation/Platform/InputListener.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class InputManager;
class MouseButtonEvent;

class InputLayer : public InputListener
{
public:
    InputLayer(i32 inputPriority);
    virtual ~InputLayer() = default;

    i32 inputPriority() const { return m_inputPriority; }

private:
    i32 m_inputPriority;
};
}