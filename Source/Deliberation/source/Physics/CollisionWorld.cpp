#include <Deliberation/Physics/CollisionWorld.h>

namespace deliberation
{

CollisionWorld::CollisionWorld()
{

}

CollisionWorld::~CollisionWorld() = default;

void CollisionWorld::addCollisionObject(const std::shared_ptr<CollisionObject> & object)
{
    m_collisionObjects.insert(object);
}

void CollisionWorld::removeCollisionObject(const CollisionObject & object)
{

}

}