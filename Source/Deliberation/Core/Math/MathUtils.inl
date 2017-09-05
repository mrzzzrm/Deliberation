#pragma once

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/FloatUtils.h>

namespace deliberation
{
template<typename T>
i32 SigNum(T value)
{
    return (T(0) < value) - (value < T(0));
}

template<typename T>
i32 Sign(T value)
{
    return value >= T(0) ? T(1) : T(-1);
}

float Lerp(float a, float b, float w)
{
    Assert(0.0f <= w && 1.0f >= w);
    return a * (1 - w) + b * w;
}

glm::quat GetArcQuaternion(glm::vec3 a, glm::vec3 b)
{
    glm::quat q;

    a = glm::normalize(a);
    b = glm::normalize(b);

    const auto d = glm::dot(b, a);
    // If dot == 1, vectors are the same
    if (d >= 1.0f)
    {
        return glm::quat();
    }
    if (d < (1e-6f - 1.0f))
    {
        // Generate an axis
        auto axis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), a);
        if (EpsilonEq(glm::length2(axis), 0.0f)) // pick another if colinear
            axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), a);

        axis = glm::normalize(axis);
        q = glm::angleAxis(glm::pi<float>(), axis);
    }
    else
    {
        const auto s = std::sqrt((1.0f + d) * 2.0f);
        const auto invs = 1.0 / s;
        const auto c = glm::cross(a, b);

        q.x = c.x * invs;
        q.y = c.y * invs;
        q.z = c.z * invs;
        q.w = s * 0.5f;
        q = glm::normalize(q);
    }

    return q;
}
}