#include <Deliberation/ImGui/ImGuiSystem.h>

#include <glbinding/gl/gl.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiRenderer.h>

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/KeyMap.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

namespace deliberation
{
ImGuiSystem::ImGuiSystem(World & world)
    : Base(world)
    , InputLayer(200)
    , m_input(world.systemRef<ApplicationSystem>().input())
{
    auto & renderManager = world.systemRef<RenderSystem>().renderManager();
    renderManager.addRenderer<ImGuiRenderer>();

    auto & io = ImGui::GetIO();

    io.RenderDrawListsFn = nullptr;

    /**
     * Setup input
     */
    io.KeyMap[ImGuiKey_Tab] = Key_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = Key_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = Key_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = Key_UP;
    io.KeyMap[ImGuiKey_DownArrow] = Key_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = Key_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = Key_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = Key_HOME;
    io.KeyMap[ImGuiKey_End] = Key_END;
    io.KeyMap[ImGuiKey_Delete] = Key_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = Key_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = Key_ENTER;
    io.KeyMap[ImGuiKey_Escape] = Key_ESCAPE;
    io.KeyMap[ImGuiKey_A] = Key_A;
    io.KeyMap[ImGuiKey_C] = Key_C;
    io.KeyMap[ImGuiKey_V] = Key_V;
    io.KeyMap[ImGuiKey_X] = Key_X;
    io.KeyMap[ImGuiKey_Y] = Key_Y;
    io.KeyMap[ImGuiKey_Z] = Key_Z;
}

void ImGuiSystem::onFrameBegin()
{
    auto & io = ImGui::GetIO();

    const auto & backbuffer =
        world().systemRef<ApplicationSystem>().drawContext().backbuffer();

    io.DisplaySize =
        ImVec2((float)backbuffer.width(), (float)backbuffer.height());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    auto mouseX =
        (m_input.mousePosition().x + 1.0f) / 2.0f * backbuffer.width();
    auto mouseY = (1.0f - (m_input.mousePosition().y + 1.0f) / 2.0f) *
                  backbuffer.height();

    // io.MouseDrawCursor = true;

    io.MousePos = ImVec2(mouseX, mouseY);

    io.MouseDown[0] = m_input.mouseButtonDown(MouseButton::Left);
    io.MouseDown[1] = m_input.mouseButtonDown(MouseButton::Right);
    io.MouseDown[2] = false;
    io.MouseDown[3] = false;
    io.MouseDown[4] = false;

    io.MouseWheel = m_mouseWheel;
    m_mouseWheel = 0.0f;

    for (size_t k = 0; k < 512; k++)
    {
        io.KeysDown[k] = false; // m_input.keyPressed(k);
    }

    io.KeyCtrl =
        io.KeysDown[Key_LEFT_CONTROL] || io.KeysDown[Key_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[Key_LEFT_SHIFT] || io.KeysDown[Key_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[Key_LEFT_ALT] || io.KeysDown[Key_RIGHT_ALT];
    io.KeySuper = io.KeysDown[Key_LEFT_SUPER] || io.KeysDown[Key_RIGHT_SUPER];

    io.DeltaTime = 1.0f / 60.0f;

    ImGui::NewFrame();

    m_wantsCaptureMouse = io.WantCaptureMouse;

    bool open = true;
    ImGui::ShowTestWindow(&open);
}

void ImGuiSystem::onMouseButtonPressed(MouseButtonEvent & event)
{
    if (m_wantsCaptureMouse) event.consume();
}

void ImGuiSystem::onMouseButtonReleased(MouseButtonEvent & event)
{
    if (m_wantsCaptureMouse) event.consume();
}

void ImGuiSystem::onMouseWheel(MouseWheelEvent & event)
{
    m_mouseWheel += event.y();

    if (m_wantsCaptureMouse) event.consume();
}

void ImGuiSystem::onMouseMotion(MouseMotionEvent & event)
{
    if (m_wantsCaptureMouse) event.consume();
}
}