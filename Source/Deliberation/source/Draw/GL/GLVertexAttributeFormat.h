#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

enum class GLVertexAttributeFormatType
{
    Format,
    IFormat,
    LFormat
};

struct GLVertexAttributeFormat
{
    gl::GLenum type;
    gl::GLuint size;
    GLVertexAttributeFormatType call;
    int numLocations;
    int innerStride;

    gl::GLuint relativeOffsetOfLocation(int location) const;
};

GLVertexAttributeFormat DELIBERATION_API GLGetVertexAttributeFormat(Type type);

}

