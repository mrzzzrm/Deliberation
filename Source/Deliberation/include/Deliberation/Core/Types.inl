#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{

template<typename T>
constexpr Type & Type::resolve()
{
    return Type_None;
}

#define GEN_RESOLVE(t, T) template<> constexpr Type & Type::resolve<t>() { return T; }

GEN_RESOLVE(u8,  Type_U8)
GEN_RESOLVE(u16, Type_U16)
GEN_RESOLVE(u32, Type_U32)
GEN_RESOLVE(i8,  Type_I8)
GEN_RESOLVE(i16, Type_I16)
GEN_RESOLVE(i32, Type_I32)
GEN_RESOLVE(float, Type_Float)
GEN_RESOLVE(double, Type_Double)
GEN_RESOLVE(glm::vec2, Type_Vec2)
GEN_RESOLVE(glm::vec3, Type_Vec3)
GEN_RESOLVE(glm::vec4, Type_Vec4)
GEN_RESOLVE(glm::ivec2, Type_IVec2)
GEN_RESOLVE(glm::ivec3, Type_IVec3)
GEN_RESOLVE(glm::ivec4, Type_IVec4)
GEN_RESOLVE(glm::uvec2, Type_UVec2)
GEN_RESOLVE(glm::uvec3, Type_UVec3)
GEN_RESOLVE(glm::uvec4, Type_UVec4)
GEN_RESOLVE(glm::mat2, Type_Mat2)
GEN_RESOLVE(glm::mat3, Type_Mat3)
GEN_RESOLVE(glm::mat4, Type_Mat4)

#undef GEN_RESOLVE

}