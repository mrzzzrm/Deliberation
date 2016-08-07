#include <Deliberation/Physics/Broadphase.h>

namespace deliberation
{

Broadphase::AABBCallback::~AABBCallback() = default;

Broadphase::Broadphase()
{

}

Broadphase::~Broadphase() = default;

std::shared_ptr<BroadphaseProxy> Broadphase::createProxy(const CollisionObject & object)
{

}

void Broadphase::removeProxy(const BroadphaseProxy & proxy)
{

}

}

