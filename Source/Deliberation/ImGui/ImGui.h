#pragma once

#include <imgui.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{
void ImGuiProperty(float & value, const char * label);

void ImGuiProperty(Transform3D &transform3D);
void ImGuiProperty(const Transform3D &transform3D);
}