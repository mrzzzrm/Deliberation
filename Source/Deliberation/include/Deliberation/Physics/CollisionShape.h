#pragma once

#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Transform3D;

enum CollisionShapeType
{
    CollisionShape_Sphere,
    CollisionShape_Box,
    CollisionShape_End
};

class DELIBERATION_API CollisionShape
{
public:
    CollisionShape(int type);
    virtual ~CollisionShape();

    int type() const;

    virtual AABB bounds(const Transform3D & transform) const = 0;

private:
    int m_type;
};

}

