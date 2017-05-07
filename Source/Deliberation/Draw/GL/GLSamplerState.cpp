#include "GLSamplerState.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Sampler.h>

namespace deliberation
{
GLSamplerState::GLSamplerState()
{
    gl::glGenSamplers(1, &m_name);
    Assert(m_name != 0, "Failed to create GL sampler object");

    m_wrap[0] = gl::GL_REPEAT;
    m_wrap[1] = gl::GL_REPEAT;
    m_wrap[2] = gl::GL_REPEAT;
}

GLSamplerState::~GLSamplerState() { gl::glDeleteSamplers(1, &m_name); }

gl::GLuint GLSamplerState::name() const { return m_name; }

void GLSamplerState::setWrap(unsigned int index, gl::GLenum wrap)
{
    Assert(index < 3, "");

    if (m_wrap[index] == wrap)
    {
        return;
    }

    static const gl::GLenum mapping[] = {
        gl::GL_TEXTURE_WRAP_S, gl::GL_TEXTURE_WRAP_T, gl::GL_TEXTURE_WRAP_R};

    gl::glSamplerParameteri(m_name, mapping[index], (gl::GLint)wrap);
    m_wrap[index] = wrap;
}

void GLSamplerState::setMinFilter(gl::GLenum filter)
{
    gl::glSamplerParameteri(
        m_name, gl::GL_TEXTURE_MIN_FILTER, (gl::GLint)filter);
}

void GLSamplerState::setMagFilter(gl::GLenum filter)
{
    gl::glSamplerParameteri(
        m_name, gl::GL_TEXTURE_MAG_FILTER, (gl::GLint)filter);
}
}
