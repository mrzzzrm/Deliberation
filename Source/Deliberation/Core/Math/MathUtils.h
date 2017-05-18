#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
glm::vec3 AnyPerpendicularVectorTo(const glm::vec3 & v);

glm::mat3 RotationMatrixFromDirectionY(const glm::vec3 & y);

glm::mat3 InverseDiagonalMatrix(const glm::mat3 & m);

glm::quat QuaternionAxisRotation(const glm::quat & q, const glm::vec3 & a);

glm::vec2 RotateHalfPiCW(const glm::vec2 & v);
glm::vec2 RotateHalfPiCCW(const glm::vec2 & v);

// 1, 0, -1
template<typename T>
i32 SigNum(T value);

// 1, -1
template<typename T>
i32 Sign(T value);


float GaussianCDFWithDenominator(float x, float denom);
std::vector<float> GaussianSamples(float std, u32 numSamples);

}

#include <Deliberation/Core/Math/MathUtils.inl>