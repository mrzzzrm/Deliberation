#include <Deliberation/Draw/GL/GLType.h>

#include <iostream>
#include <unordered_map>

#include <glbinding/gl/enum.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/TypeID.h>

using namespace gl;
using namespace deliberation;

namespace
{
std::unordered_map<u8, gl::GLenum> ELEMENTAL_TYPE_TO_GL_TYPE{
    {ELEMENTAL_TYPE_BOOL, GL_BOOL},
    {ELEMENTAL_TYPE_U8, GL_UNSIGNED_BYTE},
    {ELEMENTAL_TYPE_U16, GL_UNSIGNED_SHORT},
    {ELEMENTAL_TYPE_U32, GL_UNSIGNED_INT},
    {ELEMENTAL_TYPE_I8, GL_BYTE},
    {ELEMENTAL_TYPE_I16, GL_SHORT},
    {ELEMENTAL_TYPE_I32, GL_INT},
    {ELEMENTAL_TYPE_FLOAT, GL_FLOAT},
};

std::unordered_map<u8, gl::GLenum> TYPE_TO_GL_TYPE{
    {TYPE_BOOL, GL_BOOL},
    {TYPE_U8, GL_UNSIGNED_BYTE},
    {TYPE_U16, GL_UNSIGNED_SHORT},
    {TYPE_U32, GL_UNSIGNED_INT},
    {TYPE_I8, GL_BYTE},
    {TYPE_I16, GL_SHORT},
    {TYPE_I32, GL_INT},
    {TYPE_FLOAT, GL_FLOAT},
    {TYPE_DOUBLE, GL_DOUBLE},
    {TYPE_VEC2, GL_FLOAT_VEC2},
    {TYPE_VEC3, GL_FLOAT_VEC3},
    {TYPE_VEC4, GL_FLOAT_VEC4},
    {TYPE_IVEC2, GL_INT_VEC2},
    {TYPE_IVEC3, GL_INT_VEC3},
    {TYPE_IVEC4, GL_INT_VEC4},
    {TYPE_UVEC2, GL_UNSIGNED_INT_VEC2},
    {TYPE_UVEC3, GL_UNSIGNED_INT_VEC3},
    {TYPE_UVEC4, GL_UNSIGNED_INT_VEC4},
    {TYPE_MAT2, GL_FLOAT_MAT2},
    {TYPE_MAT3, GL_FLOAT_MAT3},
    {TYPE_MAT4, GL_FLOAT_MAT4}};

std::unordered_map<i32, u8> GL_TYPE_TO_TYPE{
    {(i32)GL_BOOL, TYPE_BOOL},
    {(i32)GL_UNSIGNED_BYTE, TYPE_U8},
    {(i32)GL_UNSIGNED_SHORT, TYPE_U16},
    {(i32)GL_UNSIGNED_INT, TYPE_U32},
    {(i32)GL_BYTE, TYPE_I8},
    {(i32)GL_SHORT, TYPE_I16},
    {(i32)GL_INT, TYPE_I32},
    {(i32)GL_FLOAT, TYPE_FLOAT},
    {(i32)GL_DOUBLE, TYPE_DOUBLE},
    {(i32)GL_FLOAT_VEC2, TYPE_VEC2},
    {(i32)GL_FLOAT_VEC3, TYPE_VEC3},
    {(i32)GL_FLOAT_VEC4, TYPE_VEC4},
    {(i32)GL_INT_VEC2, TYPE_IVEC2},
    {(i32)GL_INT_VEC3, TYPE_IVEC3},
    {(i32)GL_INT_VEC4, TYPE_IVEC4},
    {(i32)GL_UNSIGNED_INT_VEC2, TYPE_UVEC2},
    {(i32)GL_UNSIGNED_INT_VEC3, TYPE_UVEC3},
    {(i32)GL_UNSIGNED_INT_VEC4, TYPE_UVEC4},
    {(i32)GL_FLOAT_MAT2, TYPE_MAT2},
    {(i32)GL_FLOAT_MAT3, TYPE_MAT3},
    {(i32)GL_FLOAT_MAT4, TYPE_MAT4}};
}

namespace deliberation
{
gl::GLenum ElementalTypeToGLType(const ElementalType & type)
{
    auto it = ELEMENTAL_TYPE_TO_GL_TYPE.find(type.id());
    if (it == ELEMENTAL_TYPE_TO_GL_TYPE.end())
    {
        return GL_NONE;
    }
    else
    {
        return it->second;
    }
}

gl::GLenum TypeToGLType(const Type & type)
{
    auto it = TYPE_TO_GL_TYPE.find(type.id());
    if (it == TYPE_TO_GL_TYPE.end())
    {
        return GL_NONE;
    }
    else
    {
        return it->second;
    }
}

Type GLTypeToType(gl::GLenum glType)
{
    auto it = GL_TYPE_TO_TYPE.find((i32)glType);
    if (it == GL_TYPE_TO_TYPE.end())
    {
        return Type_None;
    }
    else
    {
        return it->second;
    }
}
}