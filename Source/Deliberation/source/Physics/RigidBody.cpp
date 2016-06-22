#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{

RigidBody::RigidBody(const Transform3D & transform,
                     const std::shared_ptr<CollisionShape> & shape):
    m_transform(transform),
    m_shape(shape)
{
}

const Transform3D & RigidBody::transform() const
{
    return m_transform;
}

const CollisionShape & RigidBody::shape() const
{
    return *m_shape;
}

}

