#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
inline glm::vec3 CalculateTrajectory(
    const glm::vec3 & originPosition,
    const glm::vec3 & originVelocity,
    float             bulletSpeed,
    const glm::vec3 & targetPosition,
    const glm::vec3 & targetVelocity,
    bool &            success);
}

#include <Deliberation/Core/Math/Trajectory.inl>