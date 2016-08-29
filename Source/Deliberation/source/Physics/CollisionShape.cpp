#include <Deliberation/Physics/CollisionShape.h>

namespace deliberation
{

CollisionShape::CollisionShape(int type):
    m_type(type)
{

}

CollisionShape::~CollisionShape() = default;

int CollisionShape::type() const
{
    return m_type;
}

}


