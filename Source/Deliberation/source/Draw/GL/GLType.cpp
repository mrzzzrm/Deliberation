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

std::unordered_map<u32, gl::GLenum> TYPE_TO_GL_TYPE
    {
        {Type_U8.id(), GL_UNSIGNED_BYTE},
        {Type_U16.id(), GL_UNSIGNED_SHORT},
        {Type_U32.id(), GL_UNSIGNED_INT},
        {Type_I8.id(),  GL_BYTE},
        {Type_I16.id(), GL_SHORT},
        {Type_I32.id(), GL_INT},
        {Type_Float.id(), GL_FLOAT},
        {Type_Double.id(), GL_DOUBLE},
        {Type_Vec2.id(), GL_FLOAT_VEC2},
        {Type_Vec3.id(), GL_FLOAT_VEC3},
        {Type_Vec4.id(), GL_FLOAT_VEC4},
        {Type_IVec2.id(), GL_INT_VEC2},
        {Type_IVec3.id(), GL_INT_VEC3},
        {Type_IVec4.id(), GL_INT_VEC4},
        {Type_UVec2.id(), GL_UNSIGNED_INT_VEC2},
        {Type_UVec3.id(), GL_UNSIGNED_INT_VEC3},
        {Type_UVec4.id(), GL_UNSIGNED_INT_VEC4},
        {Type_Mat2.id(), GL_FLOAT_MAT2},
        {Type_Mat3.id(), GL_FLOAT_MAT3},
        {Type_Mat4.id(), GL_FLOAT_MAT4}
    };

std::unordered_map<i32, u32> GL_TYPE_TO_TYPE
    {
        {(i32)GL_UNSIGNED_BYTE, Type_U8.id(), },
        {(i32)GL_UNSIGNED_SHORT, Type_U16.id()},
        {(i32)GL_UNSIGNED_INT, Type_U32.id()},
        {(i32)GL_BYTE, Type_I8.id()},
        {(i32)GL_SHORT, Type_I16.id()},
        {(i32)GL_INT, Type_I32.id()},
        {(i32)GL_FLOAT, Type_Float.id()},
        {(i32)GL_DOUBLE, Type_Double.id()},
        {(i32)GL_FLOAT_VEC2, Type_Vec2.id()},
        {(i32)GL_FLOAT_VEC3, Type_Vec3.id()},
        {(i32)GL_FLOAT_VEC4, Type_Vec4.id()},
        {(i32)GL_INT_VEC2, Type_IVec2.id()},
        {(i32)GL_INT_VEC3, Type_IVec3.id()},
        {(i32)GL_INT_VEC4, Type_IVec4.id()},
        {(i32)GL_UNSIGNED_INT_VEC2, Type_UVec2.id()},
        {(i32)GL_UNSIGNED_INT_VEC3, Type_UVec3.id()},
        {(i32)GL_UNSIGNED_INT_VEC4, Type_UVec4.id()},
        {(i32)GL_FLOAT_MAT2, Type_Mat2.id()},
        {(i32)GL_FLOAT_MAT3, Type_Mat3.id()},
        {(i32)GL_FLOAT_MAT4, Type_Mat4.id()}
    };

}

namespace deliberation
{

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