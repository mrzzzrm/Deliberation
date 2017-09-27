#pragma once

#include <imgui.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Platform/InputManager.h>
#include <Deliberation/Platform/InputLayer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DrawContext;
class InputManager;

class ImGuiSystem : public System<ImGuiSystem>,
                    public InputLayer
{
public:
    ImGuiSystem(World & world);

    void removeView(const std::string & name);
    bool showView(const std::string & name);

    void
         addControlItem(const std::string & name, const std::function<void()> & fn);
    void removeControlItem(const std::string & name);

    void onCreated() override { GetGlobal<InputManager>()->addLayer(shared_from_this()); }
    void onRemoved() override { GetGlobal<InputManager>()->removeLayer(shared_from_this()); }

    void onFrameBegin();

protected:
    void onMouseButtonPressed(MouseButtonEvent & event) override;
    void onMouseButtonReleased(MouseButtonEvent & event) override;
    void onMouseWheel(MouseWheelEvent & event) override;
    void onMouseMotion(MouseMotionEvent & event) override;

private:
    bool  m_wantsCaptureMouse = false;
    float m_mouseWheel = 0;

    std::unordered_map<std::string, bool>                  m_viewToggles;
    std::unordered_map<std::string, std::function<void()>> m_controlItems;
};
}