#pragma once

#include <memory>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

class DELIBERATION_API RigidBody final
{
public:
    RigidBody(const Transform3D & transform,
              const std::shared_ptr<CollisionShape> & shape);

    const Transform3D & transform() const;
    const CollisionShape & shape() const;

private:
    Transform3D                     m_transform;
    std::shared_ptr<CollisionShape> m_shape;
};

}

