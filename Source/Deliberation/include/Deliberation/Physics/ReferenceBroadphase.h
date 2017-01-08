#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Physics/Broadphase.h>

namespace deliberation
{

class DELIBERATION_API ReferenceBroadphase final:
    public Broadphase
{
public:
    ReferenceBroadphase(Narrowphase & narrowphase);

    virtual void setProxyBounds(BroadphaseProxy & proxy, const AABB & bounds) override;

    virtual std::vector<std::shared_ptr<BroadphaseProxy>> rayCast(const Ray3D & ray) const override;

    virtual void checkProximities() override;
};

}