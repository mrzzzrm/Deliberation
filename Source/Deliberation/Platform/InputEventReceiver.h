#pragma once

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/KeyEvent.h>
#include <Deliberation/Platform/MouseButtonEvent.h>
#include <Deliberation/Platform/MouseMotionEvent.h>
#include <Deliberation/Platform/MouseStateEvent.h>
#include <Deliberation/Platform/MouseWheelEvent.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class InputEventReceiver
{
public:
    virtual ~InputEventReceiver() = default;

    virtual void onMouseMotion(MouseMotionEvent & event){};
    virtual void onMouseButtonDown(MouseStateEvent & event){};
    virtual void onMouseButtonReleased(MouseButtonEvent & event){};
    virtual void onMouseButtonPressed(MouseButtonEvent & event){};
    virtual void onMouseWheel(MouseWheelEvent & event){};
    virtual void onMouseButtonClicked(MouseButtonEvent & event){};
    virtual void onKeyPressed(KeyEvent & event){};
    virtual void onKeyReleased(KeyEvent & event){};
    virtual void onInputUpdate(Input & input){};
};
}