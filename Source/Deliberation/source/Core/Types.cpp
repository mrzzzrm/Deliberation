#include <Deliberation/Core/Types.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

namespace
{

struct TypeData
{
    const char * name;
    size_t       size;
};

TypeData DATA[] = {
    {"None", 0},
    {"I8", sizeof(deliberation::i8)},
    {"I16", sizeof(deliberation::i16)},
    {"I32", sizeof(deliberation::i32)},
    {"U8", sizeof(deliberation::u8)},
    {"U16", sizeof(deliberation::u16)},
    {"U32", sizeof(deliberation::u32)},
    {"Float", sizeof(float)},
    {"Double", sizeof(double)},
    {"Vec2", sizeof(glm::vec2)},
    {"Vec3", sizeof(glm::vec3)},
    {"Vec4", sizeof(glm::vec4)},
    {"IVec2", sizeof(glm::ivec2)},
    {"IVec3", sizeof(glm::ivec3)},
    {"IVec4", sizeof(glm::ivec4)},
    {"UVec2", sizeof(glm::uvec2)},
    {"UVec3", sizeof(glm::uvec3)},
    {"UVec4", sizeof(glm::uvec4)},
    {"Mat2", sizeof(glm::mat2)},
    {"Mat3", sizeof(glm::mat3)},
    {"Mat4", sizeof(glm::mat4)}
};

}

namespace deliberation
{

Type::Type(u32 id):
    m_id(id)
{

}

const char * Type::name() const
{
    return DATA[m_id].name;
}

size_t Type::size() const
{
    return DATA[m_id].size;
}

u32 Type::id() const
{
    return m_id;
}

bool Type::operator==(const Type & other) const
{
    return m_id == other.m_id;
}

bool Type::operator!=(const Type & other) const
{
    return m_id != other.m_id;
}

Type Type_None(0);
Type Type_I8(1);
Type Type_I16(2);
Type Type_I32(3);
Type Type_U8(4);
Type Type_U16(5);
Type Type_U32(6);
Type Type_Float(7);
Type Type_Double(8);
Type Type_Vec2(9);
Type Type_Vec3(10);
Type Type_Vec4(11);
Type Type_IVec2(12);
Type Type_IVec3(13);
Type Type_IVec4(14);
Type Type_UVec2(15);
Type Type_UVec3(16);
Type Type_UVec4(17);
Type Type_Mat2(18);
Type Type_Mat3(19);
Type Type_Mat4(20);

#undef DECL

}