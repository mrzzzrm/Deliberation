#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Physics/CollisionShape.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API AABBCollisionShape:
    public CollisionShape
{
public:
    AABBCollisionShape();
    AABBCollisionShape(const glm::vec3 & halfExtent);

    const glm::vec3 & halfExtent() const;

    void setHalfExtent(const glm::vec3 & halfExtent);

private:
    glm::vec3 m_halfExtent;
};

}