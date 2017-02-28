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
    const auto & attributeType = attribute.type();
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

void DELIBERATION_API GLBindVertexAttribute(gl::GLuint vao,
                                            const ProgramInterfaceVertexAttribute & attribute,
                                            const Blob & data)
{
    const auto i = (gl::GLuint)attribute.location();

    switch(attribute.type().id())
    {
        case TYPE_I32:      gl::glVertexAttribI1iv(i, (gl::GLint*)data.ptr()); break;
        case TYPE_U32:      gl::glVertexAttribI1uiv(i, (gl::GLuint*)data.ptr()); break;
        case TYPE_FLOAT:    gl::glVertexAttrib1fv(i, (gl::GLfloat*)data.ptr()); break;
        case TYPE_DOUBLE:   gl::glVertexAttrib1dv(i, (gl::GLdouble*)data.ptr()); break;
        case TYPE_VEC2:     gl::glVertexAttrib2fv(i, (gl::GLfloat*)data.ptr()); break;
        case TYPE_VEC3:     gl::glVertexAttrib3fv(i, (gl::GLfloat*)data.ptr()); break;
        case TYPE_VEC4:     gl::glVertexAttrib4fv(i, (gl::GLfloat*)data.ptr()); break;
        case TYPE_I32VEC2:  gl::glVertexAttribI2iv(i, (gl::GLint*)data.ptr()); break;
        case TYPE_I32VEC3:  gl::glVertexAttribI3iv(i, (gl::GLint*)data.ptr()); break;
        case TYPE_I32VEC4:  gl::glVertexAttribI4iv(i, (gl::GLint*)data.ptr()); break;
        case TYPE_U32VEC2:  gl::glVertexAttribI2uiv(i, (gl::GLuint*)data.ptr()); break;
        case TYPE_U32VEC3:  gl::glVertexAttribI3uiv(i, (gl::GLuint*)data.ptr()); break;
        case TYPE_U32VEC4:  gl::glVertexAttribI4uiv(i, (gl::GLuint*)data.ptr()); break;

        default:
            Fail("Vertex attribute type not (yet?) supported");
    }
}

}


