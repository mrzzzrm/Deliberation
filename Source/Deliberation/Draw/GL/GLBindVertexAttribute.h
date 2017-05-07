#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/ProgramInterface.h>

namespace deliberation
{

class BufferImpl;
class GLStateManager;


/**
 * From Buffer
 */
void GLBindVertexAttribute(
    GLStateManager & glStateManager,
    gl::GLuint vao,
    const ProgramInterfaceVertexAttribute & attribute,
    const BufferImpl & buffer,
    u32 bufferFieldIndex,
    gl::GLuint divisor,
    gl::GLint baseOffset);

/**
 * From Value
 */
void GLBindVertexAttribute(
    gl::GLuint vao,
    const ProgramInterfaceVertexAttribute & attribute,
    const void * data);

}

