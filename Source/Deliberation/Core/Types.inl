#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{
template<typename T>
constexpr Type & Type::resolve()
{
    return Type_None;
}

#define GEN_RESOLVE(t, T)                                                      \
    template<>                                                                 \
    constexpr Type & Type::resolve<t>()                                        \
    {                                                                          \
        return T;                                                              \
    }

GEN_RESOLVE(bool, Type_Bool)
GEN_RESOLVE(u8, Type_U8)
GEN_RESOLVE(u16, Type_U16)
GEN_RESOLVE(u32, Type_U32)
GEN_RESOLVE(i8, Type_I8)
GEN_RESOLVE(i16, Type_I16)
GEN_RESOLVE(i32, Type_I32)
GEN_RESOLVE(float, Type_Float)
GEN_RESOLVE(double, Type_Double)
GEN_RESOLVE(glm::vec2, Type_Vec2)
GEN_RESOLVE(glm::vec3, Type_Vec3)
GEN_RESOLVE(glm::vec4, Type_Vec4)
GEN_RESOLVE(glm::i8vec2, Type_I8Vec2)
GEN_RESOLVE(glm::i8vec3, Type_I8Vec3)
GEN_RESOLVE(glm::i8vec4, Type_I8Vec4)
GEN_RESOLVE(glm::i16vec2, Type_I16Vec2)
GEN_RESOLVE(glm::i16vec3, Type_I16Vec3)
GEN_RESOLVE(glm::i16vec4, Type_I16Vec4)
GEN_RESOLVE(glm::ivec2, Type_IVec2)
GEN_RESOLVE(glm::ivec3, Type_IVec3)
GEN_RESOLVE(glm::ivec4, Type_IVec4)
GEN_RESOLVE(glm::u8vec2, Type_U8Vec2)
GEN_RESOLVE(glm::u8vec3, Type_U8Vec3)
GEN_RESOLVE(glm::u8vec4, Type_U8Vec4)
GEN_RESOLVE(glm::u16vec2, Type_U16Vec2)
GEN_RESOLVE(glm::u16vec3, Type_U16Vec3)
GEN_RESOLVE(glm::u16vec4, Type_U16Vec4)
GEN_RESOLVE(glm::uvec2, Type_UVec2)
GEN_RESOLVE(glm::uvec3, Type_UVec3)
GEN_RESOLVE(glm::uvec4, Type_UVec4)
GEN_RESOLVE(glm::mat2, Type_Mat2)
GEN_RESOLVE(glm::mat3, Type_Mat3)
GEN_RESOLVE(glm::mat4, Type_Mat4)

#undef GEN_RESOLVE
}