#include <Deliberation/ImGui/ImGuiSystem.h>

#include <glbinding/gl/gl.h>

#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiRenderer.h>

#include <Deliberation/Platform/Application.h>
#include <Deliberation/Platform/InputManager.h>
#include <Deliberation/Platform/KeyMap.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

namespace deliberation
{
ImGuiSystem::ImGuiSystem(World & world)
    : Base(world)
    , InputLayer(200)
    , m_inputManager(Application::instance().inputManager())
{
    activatePhases<FrameBeginPhase>();

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

void ImGuiSystem::removeView(const std::string & name)
{
    m_viewToggles.erase(name);
}

bool ImGuiSystem::showView(const std::string & name)
{
    return m_viewToggles[name];
}

void ImGuiSystem::addControlItem(
    const std::string & name, const std::function<void()> & fn)
{
    const auto iter = m_controlItems.find(name);
    AssertM(iter == m_controlItems.end(), "Already have " + name);
    m_controlItems.emplace(name, fn);
}

void ImGuiSystem::removeControlItem(const std::string & name)
{
    const auto iter = m_controlItems.find(name);
    AssertM(iter != m_controlItems.end(), "Don't have " + name);
    m_controlItems.erase(iter);
}

void ImGuiSystem::onFrameBegin()
{
    auto & io = ImGui::GetIO();

    const auto & backbuffer =
    Application::instance().drawContext().backbuffer();

    io.DisplaySize =
        ImVec2((float)backbuffer.width(), (float)backbuffer.height());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    auto mouseX =
        (m_inputManager.mousePosition().x + 1.0f) / 2.0f * backbuffer.width();
    auto mouseY = (1.0f - (m_inputManager.mousePosition().y + 1.0f) / 2.0f) *
                  backbuffer.height();

    // io.MouseDrawCursor = true;

    io.MousePos = ImVec2(mouseX, mouseY);

    io.MouseDown[0] = m_inputManager.mouseButtonDown(MouseButton::Left);
    io.MouseDown[1] = m_inputManager.mouseButtonDown(MouseButton::Right);
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

    if (showView("ImGui Demo"))
    {
        bool open = true;
        ImGui::ShowTestWindow(&open);
    }

    /**
     * Show main menu bar
     */
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            for (auto & pair : m_viewToggles)
            {
                ImGui::MenuItem(pair.first.c_str(), NULL, &pair.second);
            }
            ImGui::EndMenu();
        }

        if (m_controlItems.size() > 0)
        {
            if (ImGui::BeginMenu("Control"))
            {
                for (auto & pair : m_controlItems)
                {
                    if (ImGui::Button(pair.first.c_str()))
                    {
                        pair.second();
                    }
                }
                ImGui::EndMenu();
            }
        }

        ImGui::EndMainMenuBar();
    }
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