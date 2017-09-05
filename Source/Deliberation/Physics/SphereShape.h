#pragma once

#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

class SphereShape final : public CollisionShape
{
public:
    explicit SphereShape(float radius);

    AABB bounds(const Transform3D & transform) const override;

private:
    float m_radius;
};

}