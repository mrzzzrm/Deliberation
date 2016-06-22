#pragma once

#include <memory>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class AABB;
class BroadphaseProxy;
class RigidBody;

class DELIBERATION_API BroadphaseBase
{
public:
    class AABBCallback
    {
    public:
        virtual ~AABBCallback();
        virtual void process(const std::shared_ptr<BroadphaseProxy> & proxy) = 0;
    };

public:
    BroadphaseBase();
    virtual ~BroadphaseBase();

    std::shared_ptr<BroadphaseProxy> createProxy(const RigidBody & body);
    void removeProxy(const std::shared_ptr<BroadphaseProxy> & proxy);

    virtual void queryAABB(const AABB & box, AABBCallback & callback) = 0;
};

}

