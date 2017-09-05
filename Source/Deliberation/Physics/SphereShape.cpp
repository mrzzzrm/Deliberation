#include <Deliberation/Physics/SphereShape.h>

#include <Deliberation/Core/Math/Sphere.h>
#include <Deliberation/Core/Math/Transform3D.h>

namespace deliberation
{

SphereShape::SphereShape(float radius):
    CollisionShape(CollisionShape_Sphere),
    m_radius(radius)
{}

AABB SphereShape::bounds(const Transform3D & transform) const
{
    return AABB::containingSphere(Sphere(transform.position(), m_radius * transform.scale()));
}

}