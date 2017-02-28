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
 * @param vao
 * @param programInterface
 * @param buffer
 * @param divisor
 * @param name
 * @param baseoffset
 */
void DELIBERATION_API GLBindVertexAttribute(gl::GLuint vao,
                                            const ProgramInterface & programInterface,
                                            const detail::BufferImpl & buffer,
                                            gl::GLuint divisor,
                                            const std::string & name,
                                            gl::GLint baseoffset);

/**
 * Fixed
 * @param vao
 * @param programInterface
 * @param name
 * @param data
 */
void DELIBERATION_API GLBindVertexAttribute(gl::GLuint vao,
                                            const ProgramInterfaceVertexAttribute & attribute,
                                            const Blob & data);

}

