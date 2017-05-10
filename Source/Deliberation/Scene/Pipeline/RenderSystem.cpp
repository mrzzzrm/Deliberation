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

            auto listSurfaces = [&] (const std::string & fbName, Framebuffer & framebuffer) {
                if (!framebuffer.isInitialized()) return;

                for (auto & rt : framebuffer.colorTargets())
                {
                    const auto surfaceKey = fbName + rt.name;

                    if (ImGui::RadioButton((fbName + " - " + rt.name).c_str(), m_selectedSurfaceKey == surfaceKey))
                    {
                        m_renderManager.surfaceOverlayRenderer().showSurface(rt.surface);
                        m_selectedSurfaceKey = surfaceKey;
                    }
                }
            };

            listSurfaces("SSAO", m_renderManager.ssaoBuffer());
            listSurfaces("GBuffer", m_renderManager.gbuffer());
            listSurfaces("HDR", m_renderManager.hdrBuffer());

            // Renderer specific framebuffers
            for (auto & renderer : m_renderManager.renderers())
            {
                auto framebuffers = renderer->framebuffers();
                for (auto & framebuffer : framebuffers)
                {
                    listSurfaces(renderer->name(), framebuffer);
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