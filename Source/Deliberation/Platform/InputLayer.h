#pragma once

#include <Deliberation/Platform/InputEventReceiver.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class Input;
class MouseButtonEvent;

class InputLayer : public InputEventReceiver
{
public:
    InputLayer(i32 inputPriority);
    virtual ~InputLayer() = default;

    i32 inputPriority() const { return m_inputPriority; }

private:
    i32 m_inputPriority;
};
}