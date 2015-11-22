#pragma once

#include <glbinding/gl/types.h>

namespace deliberation
{

enum class GLVertexAttributeFormatType
{
    Format,
    IFormat,
//    LFormat
};

class GLVertexAttributeFormat final
{
public:
    GLVertexAttributeFormat(gl::GLenum type);

    gl::GLenum type() const;
    gl::GLuint size() const;

    int numLocations() const;

    gl::GLuint relativeOffsetOfLocation(int location) const;

    GLVertexAttributeFormatType call() const;

private:
    gl::GLenum m_type;
    gl::GLuint m_size;
    int m_numLocations;
    int m_innerStride;
    GLVertexAttributeFormatType m_call;
};

}

