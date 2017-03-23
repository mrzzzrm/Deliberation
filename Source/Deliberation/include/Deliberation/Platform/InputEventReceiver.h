#pragma once

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/MouseButtonEvent.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class InputEventReceiver
{
public:
    virtual ~InputEventReceiver() = default;

    virtual void onMouseButtonPressed(MouseButtonEvent & event) {};
    virtual void onMouseButtonDown(MouseButtonEvent & event) {};
    virtual void onMouseButtonClicked(MouseButtonEvent & event) {};
    virtual void onInputUpdate(Input & input) {};
};

}