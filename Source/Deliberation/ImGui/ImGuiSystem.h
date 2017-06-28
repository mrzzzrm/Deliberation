#pragma once

#include <imgui.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/InputLayer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DrawContext;
class Input;

class ImGuiSystem : public std::enable_shared_from_this<ImGuiSystem>,
                    public System<ImGuiSystem>,
                    public InputLayer
{
  public:
    ImGuiSystem(World & world);

    void removeView(const std::string & name);
    bool showView(const std::string & name);

    void onCreated() override { m_input.addLayer(shared_from_this()); }
    void onRemoved() override { m_input.removeLayer(shared_from_this()); }

  protected:
    void onFrameBegin() override;

    void onMouseButtonPressed(MouseButtonEvent & event) override;
    void onMouseButtonReleased(MouseButtonEvent & event) override;
    void onMouseWheel(MouseWheelEvent & event) override;
    void onMouseMotion(MouseMotionEvent & event) override;

  private:
    Input & m_input;

    bool  m_wantsCaptureMouse = false;
    float m_mouseWheel = 0;

    std::unordered_map<std::string, bool> m_viewToggles;
};
}