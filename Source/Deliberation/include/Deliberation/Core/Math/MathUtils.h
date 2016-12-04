#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

glm::vec3 DELIBERATION_API AnyPerpendicularVectorTo(const glm::vec3 & v);

glm::mat3 DELIBERATION_API RotationMatrixFromDirectionY(const glm::vec3 & y);

glm::mat3 DELIBERATION_API InverseDiagonalMatrix(const glm::mat3 & m);

glm::quat DELIBERATION_API QuaternionAxisRotation(const glm::quat & q, const glm::vec3 & a);

glm::vec2 DELIBERATION_API RotateHalfPiCW(const glm::vec2 & v);
glm::vec2 DELIBERATION_API RotateHalfPiCCW(const glm::vec2 & v);

}