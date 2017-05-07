#include <Deliberation/Physics/BoxShape.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{
BoxShape::BoxShape(const glm::vec3 & halfExtent)
    : CollisionShape(CollisionShape_Box), m_halfExtent(halfExtent)
{
}

const glm::vec3 & BoxShape::halfExtent() { return m_halfExtent; }

void BoxShape::setHalfExtent(const glm::vec3 & halfExtent)
{
    m_halfExtent = halfExtent;
}

Box BoxShape::instanciate(const Transform3D & transform) const
{
    auto x =
        transform.directionLocalToWorld(glm::vec3(m_halfExtent.x, 0.0f, 0.0f));
    auto y =
        transform.directionLocalToWorld(glm::vec3(0.0f, m_halfExtent.y, 0.0f));
    auto z =
        transform.directionLocalToWorld(glm::vec3(0.0f, 0.0f, m_halfExtent.z));

    return Box(transform.position(), x, y, z);
}

AABB BoxShape::bounds(const Transform3D & transform) const
{
    return instanciate(transform).bounds();
}

glm::mat3 BoxShape::localInertia() const
{
    auto extent = m_halfExtent * 2.0f;

    auto x = extent.y * extent.y + extent.z * extent.z;
    auto y = extent.x * extent.x + extent.z * extent.z;
    auto z = extent.x * extent.x + extent.y * extent.y;

    return (1.0f / 12.0f) *
           glm::mat3({x, 0.0f, 0.0f}, {0.0f, y, 0.0f}, {0.0f, 0.0f, z});
}
}