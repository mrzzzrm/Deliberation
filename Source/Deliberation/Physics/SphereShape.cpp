#include <Deliberation/Physics/SphereShape.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{
SphereShape::SphereShape(float radius)
    : CollisionShape(CollisionShape_Sphere), m_radius(radius)
{
}

float SphereShape::radius() const { return m_radius; }

AABB SphereShape::bounds(const Transform3D & transform) const
{
    auto llf = transform.position() - m_radius;
    auto urb = transform.position() + m_radius;
    return AABB(llf, urb);
}

glm::mat3 SphereShape::localInertia() const
{
    auto e = 0.4f * m_radius * m_radius;

    glm::mat3 result;
    result[0] = {e, 0.0f, 0.0f};
    result[1] = {0.0f, e, 0.0f};
    result[2] = {0.0f, 0.0f, e};

    return result;
}
}
