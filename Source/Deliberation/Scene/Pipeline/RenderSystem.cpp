#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include <imgui.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Scene/Debug/DebugSurfaceOverlayRenderer.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{
RenderSystem::RenderSystem(World & world)
    : Base(world)
    , m_renderManager(world.systemRef<ApplicationSystem>().drawContext())
{
}


void RenderSystem::onUpdate(float seconds)
{
    if (world().system<ImGuiSystem>())
    {
        if (ImGui::Begin("Surfaces"))
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

                    if (ImGui::RadioButton((framebuffer.name() + " - " + rt.name).c_str(), m_selectedSurfaceKey == surfaceKey))
                    {
                        m_renderManager.surfaceOverlayRenderer().showSurface(rt.surface);
                        m_selectedSurfaceKey = surfaceKey;
                    }
                }
            }
        }
        ImGui::End();

        if (ImGui::Begin("Renderers"))
        {
            for (auto & renderer : m_renderManager.renderers())
            {
                if (ImGui::CollapsingHeader(renderer->name().c_str()))
                {
                    renderer->renderDebugGui();
                }
            }
        }
        ImGui::End();
    }
}
}