#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/AABB.h>

namespace deliberation
{
class Transform3D;

enum CollisionShapeType
{
    CollisionShape_Sphere,
    CollisionShape_Box,
    CollisionShape_End
};

class CollisionShape
{
public:
    CollisionShape(int type);

    int type() const;

    virtual AABB      bounds(const Transform3D & transform) const = 0;
    virtual glm::mat3 localInertia() const;
    virtual float     mass() const;
    virtual glm::vec3 centerOfMass() const;

private:
    int m_type;
};
}
