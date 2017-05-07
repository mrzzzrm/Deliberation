#pragma once

#include <Deliberation/Platform/InputEventReceiver.h>
#include <Deliberation/Platform/InputLayer.h>

namespace deliberation
{
class InputLayerWrapper : public InputLayer
{
  public:
    InputLayerWrapper(InputEventReceiver & receiver, i32 inputPriority)
        : InputLayer(inputPriority), m_receiver(receiver)
    {
    }

    void onMouseMotion(MouseMotionEvent & event) override
    {
        m_receiver.onMouseMotion(event);
    }
    void onMouseButtonReleased(MouseButtonEvent & event) override
    {
        m_receiver.onMouseButtonReleased(event);
    }
    void onMouseButtonPressed(MouseButtonEvent & event) override
    {
        m_receiver.onMouseButtonPressed(event);
    }
    void onMouseWheel(MouseWheelEvent & event) override
    {
        m_receiver.onMouseWheel(event);
    }
    void onMouseButtonClicked(MouseButtonEvent & event) override
    {
        m_receiver.onMouseButtonClicked(event);
    }
    void onKeyPressed(KeyEvent & event) override
    {
        m_receiver.onKeyPressed(event);
    }
    void onKeyReleased(KeyEvent & event) override
    {
        m_receiver.onKeyReleased(event);
    }

  private:
    InputEventReceiver & m_receiver;
};
}