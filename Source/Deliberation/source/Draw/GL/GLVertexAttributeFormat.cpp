#include "GLVertexAttributeFormat.h"

#include <cassert>
#include <iostream>

#include <Deliberation/Draw/GL/GLSLSizeOf.h>

namespace deliberation
{

GLVertexAttributeFormat::GLVertexAttributeFormat(gl::GLenum type):
    m_type(type),
    m_size(0),
    m_numLocations(0),
    m_innerStride(0),
    m_call(GLVertexAttributeFormatType::Format)
{
    switch(type)
    {
    case gl::GL_BOOL:               m_type = gl::GL_BOOL;   m_size = 1; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;
    case gl::GL_INT:                m_type = gl::GL_INT;    m_size = 1; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;
    case gl::GL_UNSIGNED_INT:       m_type = gl::GL_INT;    m_size = 1; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;

    case gl::GL_FLOAT:              m_type = gl::GL_FLOAT;  m_size = 1; m_call = GLVertexAttributeFormatType::Format; m_numLocations = 1; break;

    case gl::GL_FLOAT_VEC2:         m_type = gl::GL_FLOAT; m_size = 2; m_call = GLVertexAttributeFormatType::Format; m_numLocations = 1; break;
    case gl::GL_FLOAT_VEC3:         m_type = gl::GL_FLOAT; m_size = 3; m_call = GLVertexAttributeFormatType::Format; m_numLocations = 1; break;
    case gl::GL_FLOAT_VEC4:         m_type = gl::GL_FLOAT; m_size = 4; m_call = GLVertexAttributeFormatType::Format; m_numLocations = 1; break;

    case gl::GL_INT_VEC2:           m_type = gl::GL_INT; m_size = 2; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;
    case gl::GL_INT_VEC3:           m_type = gl::GL_INT; m_size = 3; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;
    case gl::GL_INT_VEC4:           m_type = gl::GL_INT; m_size = 4; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;

    case gl::GL_UNSIGNED_INT_VEC2:  m_type = gl::GL_UNSIGNED_INT; m_size = 2; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;
    case gl::GL_UNSIGNED_INT_VEC3:  m_type = gl::GL_UNSIGNED_INT; m_size = 3; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;
    case gl::GL_UNSIGNED_INT_VEC4:  m_type = gl::GL_UNSIGNED_INT; m_size = 4; m_call = GLVertexAttributeFormatType::IFormat; m_numLocations = 1; break;

    case gl::GL_FLOAT_MAT2:         m_type = gl::GL_FLOAT; m_size = 2; m_call = GLVertexAttributeFormatType::Format; m_numLocations = 2; break;
    case gl::GL_FLOAT_MAT3:         m_type = gl::GL_FLOAT; m_size = 3; m_call = GLVertexAttributeFormatType::Format; m_numLocations = 3; break;
    case gl::GL_FLOAT_MAT4:         m_type = gl::GL_FLOAT; m_size = 4; m_call = GLVertexAttributeFormatType::Format; m_numLocations = 4; break;

    default:
        std::cout << "VertexAttributeFormat not implemented for " << type << std::endl;
        assert(false);
    }

    m_innerStride = GLSLSizeOf(m_type) * m_size;
}

gl::GLenum GLVertexAttributeFormat::type() const
{
    return m_type;
}

gl::GLuint GLVertexAttributeFormat::size() const
{
    return m_size;
}

int GLVertexAttributeFormat::numLocations() const
{
    return m_numLocations;
}

gl::GLuint GLVertexAttributeFormat::relativeOffsetOfLocation(int location) const
{
    assert(location >= 0 && location < m_numLocations);
    return location * m_innerStride;
}

GLVertexAttributeFormatType GLVertexAttributeFormat::call() const
{
    return m_call;
}

}

