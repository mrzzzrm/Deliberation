#pragma once

#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/Physics/CollisionObject.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API BroadphaseProxy final
{
public:
    BroadphaseProxy(CollisionObject * collisionObject, const AABB & bounds = AABB());

    CollisionObject * collisionObject() const;
    const AABB & bounds() const;

    void setBounds(const AABB & bounds);

private:
    CollisionObject *  m_collisionObject;
    AABB               m_bounds;
};

}