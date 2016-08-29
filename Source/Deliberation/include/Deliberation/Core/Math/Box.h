#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Box final
{
public:
    Box();
    Box(const glm::vec3 & p, const glm::vec3 & x, const glm::vec3 & y, const glm::vec3 & z);

    const glm::vec3 & p() const;
    const glm::vec3 & x() const;
    const glm::vec3 & y() const;
    const glm::vec3 & z() const;

    AABB bounds() const;

private:
    glm::vec3 m_p;
    glm::vec3 m_x;
    glm::vec3 m_y;
    glm::vec3 m_z;
};

}