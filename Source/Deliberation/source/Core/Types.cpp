#include <Deliberation/Core/Types.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{

Type::Type():
    m_name("None"),
    m_size(0),
    m_typeID(0)
{
}

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

bool Type::operator==(const Type & other) const
{
    return m_typeID == other.m_typeID;
}

bool Type::operator!=(const Type & other) const
{
    return m_typeID != other.m_typeID;
}

#define DECL_TYPE(name, type) Type Type_##name(#name, sizeof(type), TypeID::value<type>())

DECL_TYPE(I8, i8);
DECL_TYPE(I16, i16);
DECL_TYPE(I32, i32);
DECL_TYPE(U8, u8);
DECL_TYPE(U16, u16);
DECL_TYPE(U32, u32);
DECL_TYPE(Float, float);
DECL_TYPE(Double, double);
DECL_TYPE(Vec2, glm::vec2);
DECL_TYPE(Vec3, glm::vec3);
DECL_TYPE(Vec4, glm::vec4);
DECL_TYPE(IVec2, glm::ivec2);
DECL_TYPE(IVec3, glm::ivec3);
DECL_TYPE(IVec4, glm::ivec4);
DECL_TYPE(UVec2, glm::uvec2);
DECL_TYPE(UVec3, glm::uvec3);
DECL_TYPE(UVec4, glm::uvec4);
DECL_TYPE(Mat2, glm::mat2);
DECL_TYPE(Mat3, glm::mat3);
DECL_TYPE(Mat4, glm::mat4);
Type Type_None;

#undef DECL

}