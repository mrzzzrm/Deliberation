#include "GLVertexAttributeBinder.h"

#include <cassert>
#include <iostream>

#include <glbinding/Meta.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLType.h>

#include "Draw/Detail/BufferImpl.h"

#include "GLVertexAttributeFormat.h"

namespace deliberation
{

GLVertexAttributeBinder::GLVertexAttributeBinder(gl::GLuint vao,
                                                 const ProgramInterface & programInterface,
                                                 const detail::BufferImpl & buffer,
                                                 gl::GLuint divisor):
    m_vao(vao),
    m_programInterface(programInterface),
    m_buffer(buffer),
    m_divisor(divisor)
{
}

void GLVertexAttributeBinder::bind(const std::string & name, gl::GLint baseoffset)
{
    auto attribute = m_programInterface.attribute(name);
    auto bufferField = m_buffer.layout.field(name);
    auto bufferFieldType = bufferField.type();
    auto attributeType = GLTypeToType(attribute.type());

    Assert(attributeType.numColumns() == bufferField.type().numColumns() &&
               attributeType.numRows() == bufferField.type().numRows(),
           std::string("Cannot bind ") + bufferField.type().name() + " to " + attributeType.name());

    auto format = GLGetVertexAttributeFormat(bufferField.type());
    auto startLocation = attribute.location();

    Assert(startLocation >= 0, "");

    auto normalize = attributeType.isFloat() && bufferField.type().isInteger() ? gl::GL_TRUE : gl::GL_FALSE;
    auto type = bufferFieldType.isFloat() ? gl::GL_FLOAT : (bufferFieldType.isSigned() ? gl::GL_INT : gl::GL_UNSIGNED_INT);

    gl::glBindVertexArray(m_vao);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, m_buffer.glName);

    Assert(format.numLocations == 1, "Implement this");

    for (decltype(startLocation) l = startLocation; l < startLocation + format.numLocations; l++)
    {
        gl::glEnableVertexAttribArray(l);
        gl::glVertexAttribDivisor(l, m_divisor);

        auto localLocation = l - startLocation;
        auto offset = bufferField.offset() + format.relativeOffsetOfLocation(localLocation);

        if (attributeType.isFloat()) {
            gl::glVertexAttribPointer(l, format.size, format.type, normalize, m_buffer.layout.stride(),
                                      (const gl::GLvoid*)(baseoffset + offset));
        } else {
            gl::glVertexAttribIPointer(l, format.size, format.type, m_buffer.layout.stride(),
                                       (const gl::GLvoid*)(baseoffset + offset));
        }
    }
}

}


