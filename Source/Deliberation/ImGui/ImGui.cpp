#include <Deliberation/ImGui/ImGui.h>

#include <imgui.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation {
template<>
void ImGuiProperty<Transform3D>(Transform3D &transform3D) {
    auto position = transform3D.position();
    auto center = transform3D.center();
    auto scale = transform3D.scale();
    auto orientation = transform3D.orientation();

    if (ImGui::InputFloat3("Position", &position[0]))
        transform3D.setPosition(position);
    if (ImGui::InputFloat3("Center", &center[0]))
        transform3D.setCenter(position);
    if (ImGui::InputFloat("Scale", &scale)) transform3D.setScale(scale);
    if (ImGui::InputFloat4("Orientation", &orientation[0]))
        transform3D.setOrientation(orientation);
}

template<>
void ImGuiProperty<const Transform3D>(const Transform3D &transform3D) {
    auto position = transform3D.position();
    auto center = transform3D.center();
    auto scale = transform3D.scale();
    auto orientation = transform3D.orientation();

    ImGui::InputFloat3("Position", &position[0]);
    ImGui::InputFloat3("Center", &center[0]);
    ImGui::InputFloat("Scale", &scale);
    ImGui::InputFloat4("Orientation", &orientation[0]);
}
}