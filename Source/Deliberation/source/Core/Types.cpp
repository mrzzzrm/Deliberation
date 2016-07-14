#include <Deliberation/Core/Types.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{

Type::Type(const std::string & name, size_t size, TypeID::value_t typeID):
    m_name(name),
    m_size(size),
    m_typeID(typeID)
{

}

const std::string & Type::name() const
{
    return m_name;
}

size_t Type::size() const
{
    return m_size;
}

TypeID::value_t Type::typeID() const
{
    return m_typeID;
}

Type Type_I32    ("I32",   4, TypeID::value<i32>());
Type Type_Vec3   ("Vec3", 12, TypeID::value<glm::vec3>());

}