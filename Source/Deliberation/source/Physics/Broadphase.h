#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/SparseVector.h>



namespace deliberation
{

class AABB;
class BroadphaseProxy;
class CollisionObject;
class Narrowphase;
class ProximityCache;
class RigidBody;

class Broadphase
{
public:
    Broadphase(Narrowphase & narrowphase);
    virtual ~Broadphase();

    std::shared_ptr<BroadphaseProxy> createProxy(const CollisionObject & object);
    void removeProxy(const BroadphaseProxy & proxy);

    virtual std::vector<std::shared_ptr<BroadphaseProxy>> lineCast(const Ray3D & ray) const = 0;

    virtual void setProxyBounds(BroadphaseProxy & proxy, const AABB & bounds) = 0;

    virtual void checkProximities() = 0;

protected:
    Narrowphase &                                   m_narrowphase;
    SparseVector<std::shared_ptr<BroadphaseProxy>>  m_proxies;
};

}

