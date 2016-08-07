#pragma once

#include <memory>

#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API CollisionWorld
{
public:
    CollisionWorld();
    virtual ~CollisionWorld();

    void addCollisionObject(const std::shared_ptr<CollisionObject> & object);
    void removeCollisionObject(const CollisionObject & object);

private:
    SparseVector<std::shared_ptr<CollisionObject>> m_collisionObjects;
};

}