#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

glm::vec3 DELIBERATION_API AnyPerpendicularVectorTo(const glm::vec3 & v);

glm::mat3 DELIBERATION_API RotationMatrixFromDirectionY(const glm::vec3 & y);

glm::mat3 DELIBERATION_API InverseDiagonalMatrix(const glm::mat3 & m);

glm::quat DELIBERATION_API QuaternionAxisRotation(const glm::quat & q, const glm::vec3 & a);

glm::vec2 DELIBERATION_API RotateHalfPiCW(const glm::vec2 & v);
glm::vec2 DELIBERATION_API RotateHalfPiCCW(const glm::vec2 & v);

// 1, 0, -1
template <typename T>
i32 SigNum(T value);

// 1, -1
template<typename T>
i32 Sign(T value);

}

#include <Deliberation/Core/Math/MathUtils.inl>