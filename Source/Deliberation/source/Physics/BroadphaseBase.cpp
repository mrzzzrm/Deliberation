#include <Deliberation/Physics/BroadphaseBase.h>

namespace deliberation
{

BroadphaseBase::AABBCallback::~AABBCallback() = default;

BroadphaseBase::BroadphaseBase()
{

}

BroadphaseBase::~BroadphaseBase() = default;

std::shared_ptr<BroadphaseProxy> BroadphaseBase::createProxy(const RigidBody & body)
{

}

void BroadphaseBase::removeProxy(const std::shared_ptr<BroadphaseProxy> & proxy)
{

}

}

