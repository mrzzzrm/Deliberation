#include <Deliberation/Physics/CollisionObject.h>

#include <Deliberation/Physics/BroadphaseProxy.h>
#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

CollisionObject::CollisionObject(const std::shared_ptr<CollisionShape> & shape,
                                 const Transform3D & transform):
    m_shape(shape),
    m_transform(transform)
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

AABB CollisionObject::bounds() const
{
    return m_shape->bounds(m_transform);
}

const std::shared_ptr<BroadphaseProxy> & CollisionObject::proxy() const
{
    return m_proxy;
}

void CollisionObject::setShape(const std::shared_ptr<CollisionShape> & shape)
{
    m_shape = shape;
}

void CollisionObject::setTransform(const Transform3D & transform)
{
    m_transform = transform;
}

void CollisionObject::setProxy(const std::shared_ptr<BroadphaseProxy> & proxy)
{
    m_proxy = proxy;
}

}