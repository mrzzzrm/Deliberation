#include <Deliberation/Physics/BroadphaseProxy.h>

namespace deliberation
{

BroadphaseProxy::BroadphaseProxy(CollisionObject * collisionObject, const AABB & bounds):
    m_collisionObject(collisionObject),
    m_bounds(bounds)
{

}

CollisionObject * BroadphaseProxy::collisionObject() const
{
    return m_collisionObject;
}

const AABB & BroadphaseProxy::bounds() const
{
    return m_bounds;
}

void BroadphaseProxy::setBounds(const AABB & bounds)
{
    m_bounds = bounds;
}

}