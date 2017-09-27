#include <Deliberation/Scene/Pipeline/Renderer.h>

#include <Deliberation/ImGui/ImGui.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>

namespace deliberation
{
void Renderer::renderImGui()
{
    if (ImGui::Checkbox((std::string("Enabled##") + std::to_string((size_t)this)).c_str(), &m_enabled))
    {

    }

    if (m_enabled)
    {
        onRenderImGui();
    }
}

}