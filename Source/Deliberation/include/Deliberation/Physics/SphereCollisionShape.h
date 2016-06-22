#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

class DELIBERATION_API SphereCollisionShape:
    public CollisionShape
{
public:
    SphereCollisionShape(float radius);

    float radius() const;

private:
    float m_radius;
};

}

