#include <Deliberation/Physics/CollisionObject.h>

namespace deliberation
{

CollisionObject::CollisionObject(const std::shared_ptr<CollisionShape> & shape,
                                 const Transform3D & transform)
{

}

CollisionObject::~CollisionObject() = default;

const std::shared_ptr<CollisionShape> & CollisionObject::shape() const
{
    return m_shape;
}

Transform3D & CollisionObject::transform()
{
    return m_transform;
}

const Transform3D & CollisionObject::transform() const
{
    return m_transform;
}

void CollisionObject::setTransform(const Transform3D & transform)
{
    m_transform = transform;
}

}