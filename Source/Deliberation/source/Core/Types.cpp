#include <Deliberation/Core/Types.h>

#include <functional>
#include <sstream>

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/StreamUtils.h>

using namespace deliberation;

namespace
{

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
    const char *                      name;
    size_t                            size;
    std::function<std::string(const void*)> toString;
};

TypeData DATA[] = {
    {"None", 0, dummyToString},
    {"I8", sizeof(i8), toString<i8>},
    {"I16", sizeof(i16), toString<i16>},
    {"I32", sizeof(i32), toString<i32>},
    {"U8", sizeof(u8), toString<u8>},
    {"U16", sizeof(u16), toString<u16>},
    {"U32", sizeof(u32), toString<u32>},
    {"Float", sizeof(float), toString<float>},
    {"Double", sizeof(double), toString<double>},
    {"Vec2", sizeof(glm::vec2), toString<glm::vec2>},
    {"Vec3", sizeof(glm::vec3), toString<glm::vec3>},
    {"Vec4", sizeof(glm::vec4), toString<glm::vec4>},
    {"IVec2", sizeof(glm::ivec2), toString<glm::ivec2>},
    {"IVec3", sizeof(glm::ivec3), toString<glm::ivec3>},
    {"IVec4", sizeof(glm::ivec4), toString<glm::ivec4>},
    {"UVec2", sizeof(glm::uvec2), toString<glm::uvec2>},
    {"UVec3", sizeof(glm::uvec3), toString<glm::uvec3>},
    {"UVec4", sizeof(glm::uvec4), toString<glm::uvec4>},
    {"Mat2", sizeof(glm::mat2), toString<glm::mat2>},
    {"Mat3", sizeof(glm::mat3), toString<glm::mat3>},
    {"Mat4", sizeof(glm::mat4), toString<glm::mat4>}
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