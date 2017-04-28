#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/AABB.h>



namespace deliberation
{

class Transform3D;

class Box final
{
public:
    Box();
    Box(const glm::vec3 & p, const glm::vec3 & x, const glm::vec3 & y, const glm::vec3 & z);
    Box(const glm::vec3 & halfExtent, const Transform3D & transform);

    const glm::vec3 & p() const;
    const glm::vec3 & x() const;
    const glm::vec3 & y() const;
    const glm::vec3 & z() const;
    const glm::vec3 & halfExtent() const;

    const glm::mat3 & orientationMatrix() const;

    const glm::vec3 & axis(size_t i) const;

    AABB bounds() const;

    glm::vec3 point(size_t index) const;

private:
    glm::vec3 m_p;
    glm::vec3 m_halfExtent;
    glm::mat3 m_axes;
};

}