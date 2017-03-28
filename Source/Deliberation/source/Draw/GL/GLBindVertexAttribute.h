#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/ProgramInterface.h>

class Buffer;

namespace deliberation
{

namespace detail
{
    class BufferImpl;
}

/**
 * From Buffer
 */
void DELIBERATION_API GLBindVertexAttribute(
    gl::GLuint vao,
    const ProgramInterfaceVertexAttribute & attribute,
    const detail::BufferImpl & buffer,
    u32 bufferFieldIndex,
    gl::GLuint divisor,
    gl::GLint baseOffset);

/**
 * From Value
 */
void DELIBERATION_API GLBindVertexAttribute(
    gl::GLuint vao,
    const ProgramInterfaceVertexAttribute & attribute,
    const void * data);

}

