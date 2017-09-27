#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include <imgui.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Scene/Debug/DebugSurfaceOverlayRenderer.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>
#include <Deliberation/Platform/App.h>

namespace deliberation
{
RenderSystem::RenderSystem(World & world)
    : Base(world)
    , m_renderManager()
{
    m_debugGeometryRenderer =
        m_renderManager.addRenderer<DebugGeometryRenderer>();

    activatePhases<GameUpdatePhase>();
}

void RenderSystem::onGameUpdate(const UpdateFrame & updateFrame)
{
    auto imguiSystem = world().system<ImGuiSystem>();

    if (imguiSystem && imguiSystem->showView(m_imguiViewTag))
    {
        if (ImGui::Begin("Rendering"))
        {
            if (ImGui::CollapsingHeader("Surfaces"))
            {
                if (ImGui::RadioButton("Default", m_selectedSurfaceKey.empty()))
                {
                    m_renderManager.surfaceOverlayRenderer().disable();
                    m_selectedSurfaceKey.clear();
                }

                // Renderer specific framebuffers
                for (auto & framebuffer : m_renderManager.framebuffers())
                {
                    if (!framebuffer.isInitialized()) return;

                    for (auto & rt : framebuffer.colorTargets())
                    {
                        const auto surfaceKey = framebuffer.name() + rt.name;

                        if (ImGui::RadioButton(
                                (framebuffer.name() + " - " + rt.name).c_str(),
                                m_selectedSurfaceKey == surfaceKey))
                        {
                            m_renderManager.surfaceOverlayRenderer()
                                .showSurface(rt.surface);
                            m_selectedSurfaceKey = surfaceKey;
                        }
                    }
                }
            }

            if (ImGui::CollapsingHeader("Renderers"))
            {
                for (auto & renderer : m_renderManager.renderers())
                {
                    if (ImGui::CollapsingHeader(renderer->name().c_str()))
                    {
                        renderer->renderImGui();
                    }
                }
            }
        }
        ImGui::End();
    }
}
}