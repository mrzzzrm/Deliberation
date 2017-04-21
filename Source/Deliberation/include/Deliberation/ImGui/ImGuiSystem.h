#pragma once

#include <imgui.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Buffer.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/InputLayer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class DrawContext;
class InputBase;

class DELIBERATION_API ImGuiSystem:
    public std::enable_shared_from_this<ImGuiSystem>,
    public System<ImGuiSystem>,
    public InputLayer
{
public:
    ImGuiSystem(World & world);

    void onCreated() override { m_input.addLayer(shared_from_this()); }
    void onRemoved() override { m_input.removeLayer(shared_from_this()); }

protected:
    void onFrameBegin() override;
    void onRender() override;

    void onMouseButtonPressed(MouseButtonEvent & event) override;
    void onMouseButtonDown(MouseButtonEvent & event) override;

private:
    DrawContext &   m_context;
    InputBase & m_input;

    Draw        m_draw;
    Buffer      m_vertexBuffer;
    Buffer      m_indexBuffer;
    Uniform     m_projectionUniform;

    bool        m_wantsCaptureMouse = false;
};

}