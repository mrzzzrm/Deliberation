#include <Deliberation/Draw/GL/GLType.h>

#include <glbinding/gl/enum.h>

#include <unordered_map>

#include <glm/glm.hpp>

#include <Deliberation/Core/TypeID.h>
#include <Deliberation/Core/IntTypes.h>

using namespace gl;
using namespace deliberation;

namespace
{

std::unordered_map<TypeID::value_t, gl::GLenum> TYPE_TO_GL_TYPE
    {
        {TypeID::value<u8>(), GL_UNSIGNED_BYTE},
        {TypeID::value<u16>(), GL_UNSIGNED_SHORT},
        {TypeID::value<u32>(), GL_UNSIGNED_INT},
        {TypeID::value<i8>(),  GL_BYTE},
        {TypeID::value<i16>(), GL_SHORT},
        {TypeID::value<i32>(), GL_INT},
        {TypeID::value<float>(), GL_FLOAT},
        {TypeID::value<double>(), GL_DOUBLE},
        {TypeID::value<glm::vec2>(), GL_FLOAT_VEC2},
        {TypeID::value<glm::vec3>(), GL_FLOAT_VEC3},
        {TypeID::value<glm::vec4>(), GL_FLOAT_VEC4},
        {TypeID::value<glm::ivec2>(), GL_INT_VEC2},
        {TypeID::value<glm::ivec3>(), GL_INT_VEC3},
        {TypeID::value<glm::ivec4>(), GL_INT_VEC4},
        {TypeID::value<glm::uvec2>(), GL_UNSIGNED_INT_VEC2},
        {TypeID::value<glm::uvec3>(), GL_UNSIGNED_INT_VEC3},
        {TypeID::value<glm::uvec4>(), GL_UNSIGNED_INT_VEC4},
        {TypeID::value<glm::mat2>(), GL_FLOAT_MAT2},
        {TypeID::value<glm::mat3>(), GL_FLOAT_MAT3},
        {TypeID::value<glm::mat4>(), GL_FLOAT_MAT4}
    };

}

namespace deliberation
{

gl::GLenum DELIBERATION_API GLType(const Type & type)
{
    auto it = TYPE_TO_GL_TYPE.find(type.typeID());
    if (it == TYPE_TO_GL_TYPE.end())
    {
        return GL_NONE;
    }
    else
    {
        return it->second;
    }
}

}