#include <Deliberation/Physics/SphereCollisionShape.h>

#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{

SphereCollisionShape::SphereCollisionShape(float radius):
    CollisionShape(CollisionShape_Sphere),
    m_radius(radius)
{

}

float SphereCollisionShape::radius() const
{
    return m_radius;
}

AABB SphereCollisionShape::bounds(const Transform3D & transform) const
{
    auto llf = transform.position() - m_radius;
    auto urb = transform.position() + m_radius;
    return AABB(llf, urb);
}

}

