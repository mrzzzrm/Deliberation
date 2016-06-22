#include <Deliberation/Physics/SphereCollisionShape.h>

namespace deliberation
{

SphereCollisionShape::SphereCollisionShape(float radius):
    m_radius(radius)
{

}

float SphereCollisionShape::radius() const
{
    return m_radius;
}

}

