#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

inline bool DELIBERATION_API RandomBool(float probability);
inline int DELIBERATION_API RandomInt(int from, int to);
inline float DELIBERATION_API RandomFloat(float from, float to);
inline glm::vec2 DELIBERATION_API RandomVec2(float from, float to);
inline glm::vec3 DELIBERATION_API RandomVec3(float from, float to);
inline glm::vec2 DELIBERATION_API RandomUnitVec2();
inline glm::vec3 DELIBERATION_API RandomUnitVec3();
inline glm::vec3 DELIBERATION_API RandomInSphere();
inline glm::vec3 DELIBERATION_API RandomInHemisphere(const glm::vec3 & normal);

}

#include <Deliberation/Core/Math/Random.inl>
