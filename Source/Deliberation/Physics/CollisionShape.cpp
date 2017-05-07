#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{
CollisionShape::CollisionShape(int type) : m_type(type) {}

CollisionShape::~CollisionShape() = default;

int CollisionShape::type() const { return m_type; }

glm::mat3 CollisionShape::localInertia() const { return glm::mat3(1.0f); }

float CollisionShape::mass() const { return 1.0f; }

glm::vec3 CollisionShape::centerOfMass() const { return glm::vec3(0.0f); }
}
