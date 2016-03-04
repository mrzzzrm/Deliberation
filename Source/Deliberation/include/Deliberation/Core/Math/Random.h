#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

bool DELIBERATION_API RandomBool(float probability);
int DELIBERATION_API RandomInt(int from, int to);
float DELIBERATION_API RandomFloat(float from, float to);
glm::vec2 DELIBERATION_API RandomVec2(float from, float to);
glm::vec3 DELIBERATION_API RandomVec3(float from, float to);
glm::vec2 DELIBERATION_API RandomUnitVec2();
glm::vec3 DELIBERATION_API RandomUnitVec3();
glm::vec3 DELIBERATION_API RandomInSphere();
glm::vec3 DELIBERATION_API RandomInHemisphere(const glm::vec3 & normal);

}

#include <Deliberation/Core/Math/Random.inl>
