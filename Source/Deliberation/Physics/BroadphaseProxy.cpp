#include <Deliberation/Physics/BroadphaseProxy.h>

namespace deliberation
{

BroadphaseProxy::BroadphaseProxy(void * userData, const AABB & bounds):
    m_userData(userData),
    m_bounds(bounds),
    m_index(0)
{

}

void * BroadphaseProxy::userData() const
{
    return m_userData;
}

const AABB & BroadphaseProxy::bounds() const
{
    return m_bounds;
}

size_t BroadphaseProxy::index() const
{
    return m_index;
}

void BroadphaseProxy::setBounds(const AABB & bounds)
{
    m_bounds = bounds;
}

void BroadphaseProxy::setIndex(size_t index)
{
    m_index = index;
}

}