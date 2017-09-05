#include <Deliberation/Physics/CollisionShape.h>


#include <Deliberation/Physics/BulletPhysics.h>


namespace deliberation
{
CollisionShape::CollisionShape(int type) : m_type(type)
{
}

int CollisionShape::type() const { return m_type; }

glm::vec3 CollisionShape::localInertia(float scale) const { return glm::vec3(1.0f); }

float CollisionShape::mass(float scale) const { return 1.0f; }

glm::vec3 CollisionShape::centerOfMass() const { return glm::vec3(0.0f); }
}
