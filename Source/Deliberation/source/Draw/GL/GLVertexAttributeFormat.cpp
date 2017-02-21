#include "GLVertexAttributeFormat.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

#include <glbinding/Meta.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Draw/GL/GLSLSizeOf.h>

namespace 
{

using namespace deliberation;

struct BaseFormat
{
    gl::GLenum type;
    gl::GLuint size;
    GLVertexAttributeFormatType call;
    int numLocations;
};

BaseFormat GetBaseFormat(Type type)
{
    static std::unordered_map<u32, BaseFormat> BASE_FORMAT_BY_TYPE{
        {Type_Bool.id(),    {gl::GL_BOOL, 1, GLVertexAttributeFormatType::IFormat, 1}},
        {Type_I32.id(),     {gl::GL_INT, 1, GLVertexAttributeFormatType::IFormat, 1}},
        {Type_U32.id(),     {gl::GL_UNSIGNED_INT, 1, GLVertexAttributeFormatType::IFormat, 1}},
        {Type_Float.id(),    {gl::GL_FLOAT, 1, GLVertexAttributeFormatType::Format, 1}},
        {Type_Vec2.id(),    {gl::GL_FLOAT, 2, GLVertexAttributeFormatType::Format, 1}},
        {Type_Vec3.id(),    {gl::GL_FLOAT, 3, GLVertexAttributeFormatType::Format, 1}},
        {Type_Vec4.id(),    {gl::GL_FLOAT, 4, GLVertexAttributeFormatType::Format, 1}},
        {Type_IVec2.id(),    {gl::GL_INT, 2, GLVertexAttributeFormatType::Format, 1}},
        {Type_IVec3.id(),    {gl::GL_INT, 3, GLVertexAttributeFormatType::Format, 1}},
        {Type_IVec4.id(),    {gl::GL_INT, 4, GLVertexAttributeFormatType::Format, 1}},
    };

    return BASE_FORMAT_BY_TYPE[type.id()];

//    switch(type)
//    {
//        case Type_Bool:                 return ;
//        case Type_I32:                  return {gl::GL_INT, 1, GLVertexAttributeFormatType::IFormat, 1};
//        case Type_U32:                  return {gl::GL_INT, 1, GLVertexAttributeFormatType::IFormat, 1};
//
//        case Type_Float:                return {gl::GL_FLOAT, 1, GLVertexAttributeFormatType::Format, 1};
//
//        case Type_Vec2:                 return {gl::GL_FLOAT, 2, GLVertexAttributeFormatType::Format, 1};
//        case Type_Vec3:                 return {gl::GL_FLOAT, 3, GLVertexAttributeFormatType::Format, 1};
//        case Type_Vec4:                 return {gl::GL_FLOAT, 4, GLVertexAttributeFormatType::Format, 1};
//
//        case Type_IVec2:                return {gl::GL_INT, 2, GLVertexAttributeFormatType::IFormat, 1};
//        case Type_IVec3:                return {gl::GL_INT, 3, GLVertexAttributeFormatType::IFormat, 1};
//        case Type_IVec4:                return {gl::GL_INT, 4, GLVertexAttributeFormatType::IFormat, 1};
//
//        case Type_UVec2:                return {gl::GL_UNSIGNED_INT, 2, GLVertexAttributeFormatType::IFormat, 1};
//        case gl::GL_UNSIGNED_INT_VEC3:  return {gl::GL_UNSIGNED_INT, 3, GLVertexAttributeFormatType::IFormat, 1};
//        case gl::GL_UNSIGNED_INT_VEC4:  return {gl::GL_UNSIGNED_INT, 4, GLVertexAttributeFormatType::IFormat, 1};
//
//        case gl::GL_FLOAT_MAT2:         return {gl::GL_FLOAT, 2, GLVertexAttributeFormatType::Format, 2};
//        case gl::GL_FLOAT_MAT3:         return {gl::GL_FLOAT, 3, GLVertexAttributeFormatType::Format, 3};
//        case gl::GL_FLOAT_MAT4:         return {gl::GL_FLOAT, 4, GLVertexAttributeFormatType::Format, 4};
//
//        default:
//            Fail("VertexAttributeFormat not implemented for " + glbinding::Meta::getString(type));
//    }
}

}

namespace deliberation
{

gl::GLuint GLVertexAttributeFormat::relativeOffsetOfLocation(int location) const
{
    assert(location >= 0 && location < numLocations);
    return location * innerStride;
}

GLVertexAttributeFormat DELIBERATION_API GLGetVertexAttributeFormat(Type type)
{
    auto baseFormat = GetBaseFormat(type);

    GLVertexAttributeFormat format;
    format.type = baseFormat.type;
    format.size = baseFormat.size;
    format.call = baseFormat.call;
    format.numLocations = baseFormat.numLocations;
    format.innerStride = GLSLSizeOf(format.type) * format.size;
    return format;
}

}

