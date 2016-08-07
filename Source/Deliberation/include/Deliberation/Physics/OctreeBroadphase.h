#pragma once

#include <Deliberation/Physics/Broadphase.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API OctreeBroadphase:
    public Broadphase
{
public:
    OctreeBroadphase();

    virtual const std::vector<std::pair<BroadphaseProxy *, BroadphaseProxy *>> & overlappingObjects() const;

    virtual void updateOverlappingObjects();

    virtual void queryAABB(const AABB & box, AABBCallback & callback);

};

}