#include "GLBindVertexAttribute.h"

#include <cassert>
#include <iostream>

#include <glbinding/Meta.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Detail/BufferImpl.h>
#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/GL/GLType.h>

namespace deliberation
{
void GLBindVertexAttribute(
    GLStateManager &                        glStateManager,
    gl::GLuint                              vao,
    const ProgramInterfaceVertexAttribute & attribute,
    const BufferImpl &                      buffer,
    u32                                     bufferFieldIndex,
    gl::GLuint                              divisor,
    gl::GLint                               baseOffset)
{
    const auto & bufferField = buffer.layout.fields()[bufferFieldIndex];
    const auto & bufferFieldType = bufferField.type();
    const auto & bufferFieldElementalType = bufferFieldType.elementalType();
    const auto & attributeType = attribute.type();
    const auto & attributeElementalType = attributeType.elementalType();

    AssertM(
        attributeType.numColumns() == bufferField.type().numColumns() &&
            attributeType.numRows() == bufferField.type().numRows(),
        attribute.name() + ": " + std::string("Cannot bind ") +
            bufferField.type().name() + " to " + attributeType.name());

    auto startLocation = attribute.glLocation();

    Assert(startLocation >= 0);

    const auto normalize =
        attributeElementalType.isFloat() && bufferFieldElementalType.isInteger()
            ? gl::GL_TRUE
            : gl::GL_FALSE;
    const auto type = ElementalTypeToGLType(bufferFieldElementalType);
    const auto size = bufferFieldType.numRows();

    glStateManager.bindVertexArray(vao);
    glStateManager.bindBuffer(gl::GL_ARRAY_BUFFER, buffer.glName);

    for (i32 l = startLocation;
         l < startLocation + bufferFieldType.numColumns();
         l++)
    {
        gl::glEnableVertexAttribArray(l);
        gl::glVertexAttribDivisor(l, divisor);

        auto localLocation = l - startLocation;
        auto offset =
            bufferField.offset() + localLocation * bufferFieldType.numRows() *
                                       bufferFieldType.elementalType().size();

        if (attributeElementalType.isFloat())
        {
            gl::glVertexAttribPointer(
                l,
                size,
                type,
                normalize,
                buffer.layout.stride(),
                (const gl::GLvoid *)(baseOffset + offset));
        }
        else
        {
            gl::glVertexAttribIPointer(
                l,
                size,
                type,
                buffer.layout.stride(),
                (const gl::GLvoid *)(baseOffset + offset));
        }
    }
}

void GLBindVertexAttribute(
    gl::GLuint                              vao,
    const ProgramInterfaceVertexAttribute & attribute,
    const void *                            data)
{
    const auto location = (gl::GLuint)attribute.glLocation();

    switch (attribute.type().id())
    {
    case TYPE_I32: gl::glVertexAttribI1iv(location, (gl::GLint *)data); break;
    case TYPE_U32: gl::glVertexAttribI1uiv(location, (gl::GLuint *)data); break;
    case TYPE_FLOAT: gl::glVertexAttrib1fv(location, (gl::GLfloat *)data); break;
    case TYPE_DOUBLE: gl::glVertexAttrib1dv(location, (gl::GLdouble *)data); break;
    case TYPE_VEC2: gl::glVertexAttrib2fv(location, (gl::GLfloat *)data); break;
    case TYPE_VEC3: gl::glVertexAttrib3fv(location, (gl::GLfloat *)data); break;
    case TYPE_VEC4: gl::glVertexAttrib4fv(location, (gl::GLfloat *)data); break;
    case TYPE_I32VEC2: gl::glVertexAttribI2iv(location, (gl::GLint *)data); break;
    case TYPE_I32VEC3: gl::glVertexAttribI3iv(location, (gl::GLint *)data); break;
    case TYPE_I32VEC4: gl::glVertexAttribI4iv(location, (gl::GLint *)data); break;
    case TYPE_U32VEC2: gl::glVertexAttribI2uiv(location, (gl::GLuint *)data); break;
    case TYPE_U32VEC3: gl::glVertexAttribI3uiv(location, (gl::GLuint *)data); break;
    case TYPE_U32VEC4: gl::glVertexAttribI4uiv(location, (gl::GLuint *)data); break;

    case TYPE_MAT2:
        gl::glVertexAttrib3fv(location + 0, &((gl::GLfloat*)data)[0]);
        gl::glVertexAttrib3fv(location + 1, &((gl::GLfloat*)data)[2]);
        break;

    case TYPE_MAT3:
        gl::glVertexAttrib3fv(location + 0, &((gl::GLfloat*)data)[0]);
        gl::glVertexAttrib3fv(location + 1, &((gl::GLfloat*)data)[3]);
        gl::glVertexAttrib3fv(location + 2, &((gl::GLfloat*)data)[6]);
        break;

    case TYPE_MAT4:
        gl::glVertexAttrib3fv(location + 0, &((gl::GLfloat*)data)[0]);
        gl::glVertexAttrib3fv(location + 1, &((gl::GLfloat*)data)[4]);
        gl::glVertexAttrib3fv(location + 2, &((gl::GLfloat*)data)[8]);
        gl::glVertexAttrib3fv(location + 3, &((gl::GLfloat*)data)[12]);
        break;

    default: Fail("Vertex attribute type not (yet?) supported");
    }
}
}
