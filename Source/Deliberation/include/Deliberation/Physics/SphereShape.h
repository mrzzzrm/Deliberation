#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

class DELIBERATION_API SphereShape:
    public CollisionShape
{
public:
    SphereShape(float radius);

    float radius() const;

    // From CollisionShape
    virtual AABB bounds(const Transform3D & transform) const override;
    virtual glm::mat3 localInertia() const override;

private:
    float m_radius;
};

}

