#include <Deliberation/Core/Math/MathUtils.h>

#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{
glm::vec3 AnyPerpendicularVectorTo(const glm::vec3 & v)
{
    auto absV = glm::abs(v);

    if (absV.x > absV.y)
    {
        return glm::cross(v, {0.0f, 1.0f, 0.0f});
    }
    else
    {
        return glm::cross(v, {1.0f, 0.0f, 0.0f});
    }
}

glm::mat3 RotationMatrixFromDirectionY(const glm::vec3 & y)
{
    glm::mat3 result;

    result[1] = glm::normalize(y);
    result[0] = glm::normalize(AnyPerpendicularVectorTo(result[1]));
    result[2] = glm::cross(result[0], result[1]);

    return result;
}

glm::mat3 InverseDiagonalMatrix(const glm::mat3 & m)
{
    auto x = m[0][0];
    auto y = m[1][1];
    auto z = m[2][2];

    x = x != 0.0f ? 1.0f / x : 0.0f;
    y = y != 0.0f ? 1.0f / y : 0.0f;
    z = z != 0.0f ? 1.0f / z : 0.0f;

    return {{x, 0.0f, 0.0f}, {0.0f, y, 0.0f}, {0.0f, 0.0f, z}};
}

glm::quat QuaternionAxisRotation(const glm::quat & q, const glm::vec3 & a)
{
    // TODO: Epsilon
    if (a == glm::vec3(0.0f))
    {
        return q;
    }

    auto angle = glm::length(a);
    auto axis = glm::normalize(a);

    auto quat = glm::rotate(glm::quat(), angle, axis);

    return quat * q;
}

glm::vec2 RotateHalfPiCW(const glm::vec2 & v) { return {v.y, -v.x}; }

glm::vec2 RotateHalfPiCCW(const glm::vec2 & v) { return {-v.y, v.x}; }

float GaussianCDFWithDenominator(float x, float denom)
{
    return 0.5f * (1.0f + (float)erf(x / denom));
}

std::vector<float> GaussianSamples(float std, u32 numSamples)
{
    std::vector<float> result(numSamples);

    auto denom = (float)sqrt(2.0f * std * std);

    for (int s = 0; s < numSamples; s++)
    {
        result[s] = GaussianCDFWithDenominator((float)s + 0.5f, denom) -
                    GaussianCDFWithDenominator((float)s - 0.5f, denom);
    }

    return result;
}
}