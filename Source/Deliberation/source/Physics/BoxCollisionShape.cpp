#include <Deliberation/Physics/BoxCollisionShape.h>

namespace deliberation
{

BoxCollisionShape::BoxCollisionShape(const glm::vec3 & halfExtent):
    m_halfExtent(halfExtent)
{

}

const glm::vec3 & BoxCollisionShape::halfExtent() const
{
    return m_halfExtent;
}

}

