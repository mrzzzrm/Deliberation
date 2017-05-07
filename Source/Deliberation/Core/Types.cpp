#include <Deliberation/Core/Types.h>

#include <functional>
#include <sstream>

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/StreamUtils.h>

using namespace deliberation;

namespace
{
std::string dummyToString(const void *) { return ""; }

template<typename T>
std::string toString(const void * ptr)
{
    std::stringstream stream;
    stream << *((const T *)ptr);
    return stream.str();
}

struct TypeData
{
    const char *                             name;
    size_t                                   size;
    std::function<std::string(const void *)> toString;
    u8                                       numRows;
    u8                                       numColumns;
    ElementalType &                          elementalType;
};

TypeData TYPE_DATA[] = {
    {"None", 0, dummyToString, 0, 0, ElementalType_None},
    {"I8", sizeof(i8), toString<i8>, 1, 1, ElementalType_I8},
    {"I16", sizeof(i16), toString<i16>, 1, 1, ElementalType_I16},
    {"I32", sizeof(i32), toString<i32>, 1, 1, ElementalType_I32},
    {"U8", sizeof(u8), toString<u8>, 1, 1, ElementalType_U8},
    {"U16", sizeof(u16), toString<u16>, 1, 1, ElementalType_U16},
    {"U32", sizeof(u32), toString<u32>, 1, 1, ElementalType_U32},
    {"Float", sizeof(float), toString<float>, 1, 1, ElementalType_Float},
    {"Double", sizeof(double), toString<double>, 1, 1, ElementalType_Float},
    {"Vec2", sizeof(glm::vec2), toString<glm::vec2>, 2, 1, ElementalType_Float},
    {"Vec3", sizeof(glm::vec3), toString<glm::vec3>, 3, 1, ElementalType_Float},
    {"Vec4", sizeof(glm::vec4), toString<glm::vec4>, 4, 1, ElementalType_Float},
    {"I8Vec2",
     sizeof(glm::i8vec2),
     toString<glm::i8vec2>,
     2,
     1,
     ElementalType_I8},
    {"I8Vec3",
     sizeof(glm::i8vec3),
     toString<glm::i8vec3>,
     3,
     1,
     ElementalType_I8},
    {"I8Vec4",
     sizeof(glm::i8vec4),
     toString<glm::i8vec4>,
     4,
     1,
     ElementalType_I8},
    {"I16Vec2",
     sizeof(glm::i16vec2),
     toString<glm::i16vec2>,
     2,
     1,
     ElementalType_I16},
    {"I16Vec3",
     sizeof(glm::i16vec3),
     toString<glm::i16vec3>,
     3,
     1,
     ElementalType_I16},
    {"I16Vec4",
     sizeof(glm::i16vec4),
     toString<glm::i16vec4>,
     4,
     1,
     ElementalType_I16},
    {"IVec2",
     sizeof(glm::i32vec2),
     toString<glm::i32vec2>,
     2,
     1,
     ElementalType_I32},
    {"IVec3",
     sizeof(glm::i32vec3),
     toString<glm::i32vec3>,
     3,
     1,
     ElementalType_I32},
    {"IVec4",
     sizeof(glm::i32vec4),
     toString<glm::i32vec4>,
     4,
     1,
     ElementalType_I32},
    {"U8Vec2",
     sizeof(glm::u8vec2),
     toString<glm::u8vec2>,
     2,
     1,
     ElementalType_U8},
    {"U8Vec3",
     sizeof(glm::u8vec3),
     toString<glm::u8vec3>,
     3,
     1,
     ElementalType_U8},
    {"U8Vec4",
     sizeof(glm::u8vec4),
     toString<glm::u8vec4>,
     4,
     1,
     ElementalType_U8},
    {"U16Vec2",
     sizeof(glm::u16vec2),
     toString<glm::u16vec2>,
     2,
     1,
     ElementalType_U16},
    {"U16Vec3",
     sizeof(glm::u16vec3),
     toString<glm::u16vec3>,
     3,
     1,
     ElementalType_U16},
    {"u16Vec4",
     sizeof(glm::u16vec4),
     toString<glm::u16vec4>,
     4,
     1,
     ElementalType_U16},
    {"UVec2",
     sizeof(glm::u32vec2),
     toString<glm::u32vec2>,
     2,
     1,
     ElementalType_U32},
    {"UVec3",
     sizeof(glm::u32vec3),
     toString<glm::u32vec3>,
     3,
     1,
     ElementalType_U32},
    {"UVec4",
     sizeof(glm::u32vec4),
     toString<glm::u32vec4>,
     4,
     1,
     ElementalType_U32},
    {"Mat2", sizeof(glm::mat2), toString<glm::mat2>, 2, 2, ElementalType_Float},
    {"Mat3", sizeof(glm::mat3), toString<glm::mat3>, 3, 3, ElementalType_Float},
    {"Mat4", sizeof(glm::mat4), toString<glm::mat4>, 4, 4, ElementalType_Float},
    {"Bool", sizeof(bool), toString<bool>, 1, 1, ElementalType_Bool}};
}

namespace deliberation
{
Type::Type(u8 id) : m_id(id) {}

const char * Type::name() const { return TYPE_DATA[m_id].name; }

size_t Type::size() const { return TYPE_DATA[m_id].size; }

u32 Type::id() const { return m_id; }

u8 Type::numRows() const { return TYPE_DATA[m_id].numRows; }

u8 Type::numColumns() const { return TYPE_DATA[m_id].numColumns; }

ElementalType Type::elementalType() const
{
    return TYPE_DATA[m_id].elementalType;
}

std::string Type::toString(const void * ptr) const
{
    return TYPE_DATA[m_id].toString(ptr);
}

bool Type::operator==(const Type & other) const { return m_id == other.m_id; }

bool Type::operator!=(const Type & other) const { return m_id != other.m_id; }

Type Type_None(TYPE_NONE);
Type Type_I8(TYPE_U8);
Type Type_I16(TYPE_I16);
Type Type_I32(TYPE_I32);
Type Type_U8(TYPE_U8);
Type Type_U16(TYPE_U16);
Type Type_U32(TYPE_U32);
Type Type_Float(TYPE_FLOAT);
Type Type_Double(TYPE_DOUBLE);
Type Type_Vec2(TYPE_VEC2);
Type Type_Vec3(TYPE_VEC3);
Type Type_Vec4(TYPE_VEC4);
Type Type_I8Vec2(TYPE_I8VEC2);
Type Type_I8Vec3(TYPE_I8VEC3);
Type Type_I8Vec4(TYPE_I8VEC4);
Type Type_I16Vec2(TYPE_I16VEC2);
Type Type_I16Vec3(TYPE_I16VEC3);
Type Type_I16Vec4(TYPE_I16VEC4);
Type Type_I32Vec2(TYPE_I32VEC2);
Type Type_I32Vec3(TYPE_I32VEC3);
Type Type_I32Vec4(TYPE_I32VEC4);
Type Type_IVec2(TYPE_IVEC2);
Type Type_IVec3(TYPE_IVEC3);
Type Type_IVec4(TYPE_IVEC4);
Type Type_U8Vec2(TYPE_U8VEC2);
Type Type_U8Vec3(TYPE_U8VEC3);
Type Type_U8Vec4(TYPE_U8VEC4);
Type Type_U16Vec2(TYPE_U16VEC2);
Type Type_U16Vec3(TYPE_U16VEC3);
Type Type_U16Vec4(TYPE_U16VEC4);
Type Type_U32Vec2(TYPE_U32VEC2);
Type Type_U32Vec3(TYPE_U32VEC3);
Type Type_U32Vec4(TYPE_U32VEC4);
Type Type_UVec2(TYPE_UVEC2);
Type Type_UVec3(TYPE_UVEC3);
Type Type_UVec4(TYPE_UVEC4);
Type Type_Mat2(TYPE_MAT2);
Type Type_Mat3(TYPE_MAT3);
Type Type_Mat4(TYPE_MAT4);
Type Type_Bool(TYPE_BOOL);
}