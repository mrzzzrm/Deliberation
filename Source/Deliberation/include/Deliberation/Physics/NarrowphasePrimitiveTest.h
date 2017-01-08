#pragma once

#include <memory>

namespace deliberation
{

class Ray3D;
class RigidBody;

struct RayCastIntersection
{
    RayCastIntersection(const RigidBody & body);
    virtual ~RayCastIntersection() = default;

    const RigidBody & body;
};

class NarrowphasePrimitiveTest
{
public:
    virtual std::unique_ptr<RayCastIntersection> rayCast(const Ray3D & ray,
                                                         const RigidBody & body) const = 0;
};

}