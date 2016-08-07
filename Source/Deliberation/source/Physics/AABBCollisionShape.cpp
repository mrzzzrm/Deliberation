#include <Deliberation/Physics/AABBCollisionShape.h>

namespace deliberation
{

AABBCollisionShape::AABBCollisionShape()
{

}

AABBCollisionShape::AABBCollisionShape(const glm::vec3 & halfExtent):
    m_halfExtent(halfExtent)
{

}

const glm::vec3 & AABBCollisionShape::halfExtent() const
{
    return m_halfExtent;
}

void AABBCollisionShape::setHalfExtent(const glm::vec3 & halfExtent)
{
    m_halfExtent = halfExtent;
}

}