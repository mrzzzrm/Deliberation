#pragma once

#include <memory>

namespace deliberation
{
class Ray3D;
class RigidBody;

struct RayCastIntersection
{
    RayCastIntersection(std::shared_ptr<RigidBody> body);
    virtual ~RayCastIntersection() = default;

    std::shared_ptr<RigidBody> body;
};

class PrimitiveTestAlgorithm
{
public:
    virtual std::unique_ptr<RayCastIntersection>
    lineTest(const Ray3D & ray, std::shared_ptr<RigidBody> body) const = 0;
};
}