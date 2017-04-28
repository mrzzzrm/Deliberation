#include <Deliberation/Physics/Broadphase.h>

#include <Deliberation/Physics/BroadphaseProxy.h>
#include <Deliberation/Physics/CollisionObject.h>

namespace deliberation
{

Broadphase::Broadphase(Narrowphase & narrowphase):
    m_narrowphase(narrowphase)
{

}

Broadphase::~Broadphase() = default;

std::shared_ptr<BroadphaseProxy> Broadphase::createProxy(const CollisionObject & collisionObject)
{
    auto proxy = std::make_shared<BroadphaseProxy>((void*)&collisionObject, collisionObject.bounds());
    auto index = m_proxies.insert(proxy);
    proxy->setIndex(index);
    return proxy;
}

void Broadphase::removeProxy(const BroadphaseProxy & proxy)
{
    auto index = proxy.index();
    m_proxies.erase(index);
}

}

