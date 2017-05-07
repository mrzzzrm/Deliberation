#include <Deliberation/Physics/ReferenceBroadphase.h>

#include <iostream>

#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/BroadphaseProxy.h>
#include <Deliberation/Physics/Narrowphase.h>

namespace deliberation
{
ReferenceBroadphase::ReferenceBroadphase(Narrowphase & narrowphase)
    : Broadphase(narrowphase)
{
}

void ReferenceBroadphase::setProxyBounds(
    BroadphaseProxy & proxy, const AABB & bounds)
{
    proxy.setBounds(bounds);
}

std::vector<std::shared_ptr<BroadphaseProxy>>
ReferenceBroadphase::lineCast(const Ray3D & ray) const
{
    std::vector<std::shared_ptr<BroadphaseProxy>> result;

    for (auto & proxy : m_proxies)
    {
        auto & aabb = proxy->bounds();
        auto   position = aabb.center();
        auto   radius = aabb.diameter() / 2.0f;

        if (LineSphereIntersection(
                ray.origin(), ray.direction(), position, radius))
        {
            result.emplace_back(proxy);
        }
    }

    return result;
}

void ReferenceBroadphase::checkProximities()
{
    for (auto pA = 0; pA < m_proxies.capacity(); pA++)
    {
        if (!m_proxies.contains(pA))
        {
            continue;
        }

        auto & proxyA = m_proxies[pA];
        auto * userDataA = proxyA->userData();

        for (auto pB = pA + 1; pB < m_proxies.capacity(); pB++)
        {
            if (!m_proxies.contains(pB))
            {
                continue;
            }

            auto & proxyB = m_proxies[pB];
            auto * userDataB = proxyB->userData();

            if (proxyA->bounds().intersects(proxyB->bounds()))
            {
                if (!m_narrowphase.hasContact(userDataA, userDataB))
                {
                    m_narrowphase.addContact(userDataA, userDataB);
                }
            }
            else
            {
                if (m_narrowphase.hasContact(userDataA, userDataB))
                {
                    m_narrowphase.removeContact(userDataA, userDataB);
                }
            }
        }
    }
}
}
