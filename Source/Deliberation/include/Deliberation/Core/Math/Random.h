#pragma once

#include <glm/glm.hpp>



namespace deliberation
{

inline bool RandomBool(float probability);
inline int RandomInt(int from, int to);
inline float RandomFloat(float from, float to);
inline glm::vec2 RandomVec2(float from, float to);
inline glm::vec3 RandomVec3(float from, float to);
inline glm::vec2 RandomUnitVec2();
inline glm::vec3 RandomUnitVec3();
inline glm::vec3 RandomInSphere();
inline glm::vec2 RandomOnCircle();
inline glm::vec3 RandomInHemisphere(const glm::vec3 & normal);

}

#include <Deliberation/Core/Math/Random.inl>
