#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

glm::vec3 AnyPerpendicularVectorTo(const glm::vec3 & v);

glm::mat3 DELIBERATION_API RotationMatrixFromDirectionY(const glm::vec3 & y);

}