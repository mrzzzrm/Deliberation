#include <cmath>

#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/Math/FloatUtils.h>

namespace deliberation
{
glm::vec3 CalculateTrajectory(
    const glm::vec3 & originPosition,
    const glm::vec3 & originVelocity,
    float             bulletSpeed,
    const glm::vec3 & targetPosition,
    const glm::vec3 & targetVelocity,
    bool &            success)
{
    const auto v = originVelocity - targetVelocity; // relative velocity
    const auto p = targetPosition - originPosition; // relative position

    const auto p2 = glm::length2(p);
    const auto v2 = glm::length2(v);

    if (EpsilonEq(p2, 0.0f))
    {
        success = false;
        return {};
    }

    const auto pl = std::sqrt(p2);
    const auto pn = p / pl;

    if (EpsilonEq(v2, 0.0f))
    {
        success = true;
        return pn * bulletSpeed;
    }

    const auto vl = std::sqrt(v2);
    const auto vn = v / vl;
    const auto alpha = glm::angle(vn, pn);

    const auto h = std::sin(alpha) * vl;

    if (h > bulletSpeed)
    {
        success = false;
        return {};
    }

    const auto c1 = std::cos(alpha) * vl;
    const auto c2 = std::sqrt(bulletSpeed * bulletSpeed - h * h);

    const auto r = pn * (c1 + c2);

    success = true;
    return r - v;
}
}