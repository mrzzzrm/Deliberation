#include <Deliberation/ECS/Components.h>

#include <Deliberation/ImGui/ImGui.h>

namespace deliberation
{

void Transform3DComponent::renderImGui() 
{
    ImGuiProperty(value());
}

}