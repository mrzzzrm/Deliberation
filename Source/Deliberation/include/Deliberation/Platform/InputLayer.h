#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Input;
class MouseButtonEvent;

class InputLayer
{
public:
    InputLayer(i32 inputPriority);
    virtual ~InputLayer() = default;

    i32 inputPriority() const { return m_inputPriority; }

    virtual void onMouseButtonPressed(MouseButtonEvent & event) {};
    virtual void onMouseButtonDown(MouseButtonEvent & event) {};
    virtual void onMouseButtonClicked(MouseButtonEvent & event) {};
    virtual void onInputUpdate(Input & input) {};

private:
    i32     m_inputPriority;
};

}