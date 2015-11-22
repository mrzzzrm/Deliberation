#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/ProgramInterface.h>

class Buffer;

namespace deliberation
{

class GLVertexAttributeBinder final
{
public:
    GLVertexAttributeBinder(gl::GLuint vao,
                            const ProgramInterface & programInterface,
                            const Buffer & buffer,
                            gl::GLuint divisor = 0);

    void bind(const std::string & name, gl::GLint baseoffset = 0);
    void bindAll(gl::GLint baseoffset = 0);

private:
    gl::GLuint m_vao;
    const ProgramInterface & m_programInterface;
    const Buffer & m_buffer;
    gl::GLuint m_divisor;
};

}

