#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

class DELIBERATION_API BoxCollisionShape:
    public CollisionShape
{
public:
    BoxCollisionShape(const glm::vec3 & halfExtent);

    const glm::vec3 & halfExtent() const;

private:
    glm::vec3 m_halfExtent;
};

}

