#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

class DELIBERATION_API SphereCollisionShape:
    public CollisionShape
{
public:
    SphereCollisionShape(float radius);

    float radius() const;

    // From CollisionShape
    virtual AABB bounds(const Transform3D & transform) const override;

private:
    float m_radius;
};

}

