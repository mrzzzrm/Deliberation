#include "GLBindVertexAttribute.h"

#include <cassert>
#include <iostream>

#include <glbinding/Meta.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLType.h>

#include "Draw/Detail/BufferImpl.h"

namespace deliberation
{
void GLBindVertexAttribute(
    gl::GLuint vao,
    const ProgramInterface & programInterface,
    const detail::BufferImpl & buffer,
    gl::GLuint divisor,
    const std::string & name,
    gl::GLint baseoffset)
{
    const auto & attribute = programInterface.attribute(name);
    const auto & bufferField = buffer.layout.field(name);
    const auto & bufferFieldType = bufferField.type();
    const auto & bufferFieldElementalType = bufferFieldType.elementalType();
    const auto & attributeType = GLTypeToType(attribute.type());
    const auto & attributeElementalType = attributeType.elementalType();

    Assert(attributeType.numColumns() == bufferField.type().numColumns() &&
           attributeType.numRows() == bufferField.type().numRows(),
           std::string("Cannot bind ") + bufferField.type().name() + " to " + attributeType.name());

    auto startLocation = attribute.location();

    Assert(startLocation >= 0, "");

    const auto normalize = attributeElementalType.isFloat() && bufferFieldElementalType.isInteger() ?
                           gl::GL_TRUE : gl::GL_FALSE;
    const auto type = ElementalTypeToGLType(bufferFieldElementalType);
    const auto size = bufferFieldType.numRows();

    gl::glBindVertexArray(vao);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, buffer.glName);

    Assert(bufferFieldType.numColumns() == 1, "Implement this");

    for (decltype(startLocation) l = startLocation; l < startLocation + bufferFieldType.numColumns(); l++)
    {
        gl::glEnableVertexAttribArray(l);
        gl::glVertexAttribDivisor(l, divisor);

        auto localLocation = l - startLocation;
        auto offset = bufferField.offset()/* + format.relativeOffsetOfLocation(localLocation)*/;

        if (attributeElementalType.isFloat()) {
            gl::glVertexAttribPointer(l, size, type, normalize, buffer.layout.stride(),
                                      (const gl::GLvoid*)(baseoffset + offset));
        } else {
            gl::glVertexAttribIPointer(l, size, type, buffer.layout.stride(),
                                       (const gl::GLvoid*)(baseoffset + offset));
        }
    } 
}


}


