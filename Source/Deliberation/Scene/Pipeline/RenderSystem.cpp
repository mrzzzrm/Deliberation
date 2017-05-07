#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include <imgui.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

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