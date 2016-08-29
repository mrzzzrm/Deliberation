#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Box.h>

#include <Deliberation/Physics/CollisionShape.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API BoxShape final:
    public CollisionShape
{
public:
    BoxShape(const glm::vec3 & halfExtent = glm::vec3());

    const glm::vec3 & halfExtent();

    void setHalfExtent(const glm::vec3 & halfExtent);

    Box instanciate(const Transform3D & transform) const;

    virtual AABB bounds(const Transform3D & transform) const override;

private:
    glm::vec3 m_halfExtent;
};

}