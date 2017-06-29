#include <Deliberation/ImGui/ImGui.h>

#include <imgui.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{

template<>
void ImGuiProperty<Transform3D>(Transform3D & transform3D)
{
    auto position = transform3D.position();

    if (ImGui::InputFloat3("Position", &position[0])) transform3D.setPosition(position);
}

}