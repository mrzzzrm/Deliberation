#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class AABB;
class BroadphaseProxy;
class CollisionObject;
class RigidBody;

class DELIBERATION_API Broadphase
{
public:
    class AABBCallback
    {
    public:
        virtual ~AABBCallback();
        virtual void process(const std::shared_ptr<BroadphaseProxy> & proxy) = 0;
    };

public:
    Broadphase();
    virtual ~Broadphase();

    std::shared_ptr<BroadphaseProxy> createProxy(const CollisionObject & object);
    void removeProxy(const BroadphaseProxy & proxy);

    virtual const std::vector<std::pair<BroadphaseProxy *, BroadphaseProxy *>> & overlappingObjects() const = 0;

    virtual void setProxyBounds(BroadphaseProxy & proxy, const AABB & bounds) = 0;

    virtual void updateOverlappingObjects() = 0;

    virtual void queryAABB(const AABB & box, AABBCallback & callback) = 0;
};

}

