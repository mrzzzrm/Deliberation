#include <Deliberation/Core/Types.h>

#include <functional>
#include <sstream>

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/StreamUtils.h>

using namespace deliberation;

namespace
{

enum class ValueType
{
    None,
    Float,
    SignedInteger,
    UnsignedInteger
};

std::string dummyToString(const void*)
{
    return "";
}

template<typename T>
std::string toString(const void * ptr)
{
    std::stringstream stream;
    stream << *((const T*)ptr);
    return stream.str();
}

struct TypeData
{
    const char *    name;
    size_t          size;
    std::function<std::string(const void*)>
                    toString;
    u8              numRows;
    u8              numColumns;
    ValueType       valueType;
};

TypeData DATA[] = {
    {"None", 0, dummyToString, 0, 0, ValueType::None},
    {"I8", sizeof(i8), toString<i8>, 1, 1, ValueType::SignedInteger},
    {"I16", sizeof(i16), toString<i16>, 1, 1, ValueType::SignedInteger},
    {"I32", sizeof(i32), toString<i32>, 1, 1, ValueType::SignedInteger},
    {"U8", sizeof(u8), toString<u8>, 1, 1, ValueType::UnsignedInteger},
    {"U16", sizeof(u16), toString<u16>, 1, 1, ValueType::UnsignedInteger},
    {"U32", sizeof(u32), toString<u32>, 1, 1, ValueType::UnsignedInteger},
    {"Float", sizeof(float), toString<float>, 1, 1, ValueType::Float},
    {"Double", sizeof(double), toString<double>, 1, 1, ValueType::Float},
    {"Vec2", sizeof(glm::vec2), toString<glm::vec2>, 2, 1, ValueType::Float},
    {"Vec3", sizeof(glm::vec3), toString<glm::vec3>, 3, 1, ValueType::Float},
    {"Vec4", sizeof(glm::vec4), toString<glm::vec4>, 4, 1, ValueType::Float},
    {"I8Vec2", sizeof(glm::i8vec2), toString<glm::i8vec2>, 2, 1, ValueType::SignedInteger},
    {"I8Vec3", sizeof(glm::i8vec3), toString<glm::i8vec3>, 3, 1, ValueType::SignedInteger},
    {"I8Vec4", sizeof(glm::i8vec4), toString<glm::i8vec4>, 4, 1, ValueType::SignedInteger},
    {"I16Vec2", sizeof(glm::i16vec2), toString<glm::i16vec2>, 2, 1, ValueType::SignedInteger},
    {"I16Vec3", sizeof(glm::i16vec3), toString<glm::i16vec3>, 3, 1, ValueType::SignedInteger},
    {"I16Vec4", sizeof(glm::i16vec4), toString<glm::i16vec4>, 4, 1, ValueType::SignedInteger},
    {"IVec2", sizeof(glm::i32vec2), toString<glm::i32vec2>, 2, 1, ValueType::SignedInteger},
    {"IVec3", sizeof(glm::i32vec3), toString<glm::i32vec3>, 3, 1, ValueType::SignedInteger},
    {"IVec4", sizeof(glm::i32vec4), toString<glm::i32vec4>, 4, 1, ValueType::SignedInteger},
    {"U8Vec2", sizeof(glm::u8vec2), toString<glm::u8vec2>, 2, 1, ValueType::UnsignedInteger},
    {"U8Vec3", sizeof(glm::u8vec3), toString<glm::u8vec3>, 3, 1, ValueType::UnsignedInteger},
    {"U8Vec4", sizeof(glm::u8vec4), toString<glm::u8vec4>, 4, 1, ValueType::UnsignedInteger},
    {"U16Vec2", sizeof(glm::u16vec2), toString<glm::u16vec2>, 2, 1, ValueType::UnsignedInteger},
    {"U16Vec3", sizeof(glm::u16vec3), toString<glm::u16vec3>, 3, 1, ValueType::UnsignedInteger},
    {"u16Vec4", sizeof(glm::u16vec4), toString<glm::u16vec4>, 4, 1, ValueType::UnsignedInteger},
    {"UVec2", sizeof(glm::u32vec2), toString<glm::u32vec2>, 2, 1, ValueType::UnsignedInteger},
    {"UVec3", sizeof(glm::u32vec3), toString<glm::u32vec3>, 3, 1, ValueType::UnsignedInteger},
    {"UVec4", sizeof(glm::u32vec4), toString<glm::u32vec4>, 4, 1, ValueType::UnsignedInteger},
    {"Mat2", sizeof(glm::mat2), toString<glm::mat2>, 2, 2, ValueType::Float},
    {"Mat3", sizeof(glm::mat3), toString<glm::mat3>, 3, 3, ValueType::Float},
    {"Mat4", sizeof(glm::mat4), toString<glm::mat4>, 4, 4, ValueType::Float},
    {"Bool", sizeof(bool), toString<bool>, 1, 1, ValueType::UnsignedInteger}
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

u8 Type::numRows() const
{
    return DATA[m_id].numRows;
}

u8 Type::numColumns() const
{
    return DATA[m_id].numColumns;
}

bool Type::isFloat() const
{
    return DATA[m_id].valueType == ValueType::Float;
}

bool Type::isInteger() const
{
    return DATA[m_id].valueType == ValueType::UnsignedInteger || DATA[m_id].valueType == ValueType::SignedInteger;
}

bool Type::isSigned() const
{
    return DATA[m_id].valueType == ValueType::SignedInteger;
}

std::string Type::toString(const void * ptr) const
{
    return DATA[m_id].toString(ptr);
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
Type Type_I8Vec2(12);
Type Type_I8Vec3(13);
Type Type_I8Vec4(14);
Type Type_I16Vec2(15);
Type Type_I16Vec3(16);
Type Type_I16Vec4(17);
Type Type_I32Vec2(18);
Type Type_I32Vec3(19);
Type Type_I32Vec4(20);
Type Type_IVec2(18);
Type Type_IVec3(19);
Type Type_IVec4(20);
Type Type_U8Vec2(21);
Type Type_U8Vec3(22);
Type Type_U8Vec4(23);
Type Type_U16Vec2(24);
Type Type_U16Vec3(25);
Type Type_U16Vec4(26);
Type Type_U32Vec2(27);
Type Type_U32Vec3(28);
Type Type_U32Vec4(29);
Type Type_UVec2(27);
Type Type_UVec3(28);
Type Type_UVec4(29);
Type Type_Mat2(30);
Type Type_Mat3(31);
Type Type_Mat4(32);
Type Type_Bool(33);

}