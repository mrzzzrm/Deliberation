#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API BufferLayoutField final
{
public:
    BufferLayoutField(const std::string & name,
                      gl::GLint size,
                      gl::GLenum type,
                      gl::GLuint offset);

    const std::string & name() const;
    gl::GLint size() const;
    gl::GLenum type() const;
    gl::GLuint offset() const;

    void setName(const std::string & name);
    void setSize(gl::GLint size);
    void setType(gl::GLenum type);
    void setOffset(const gl::GLuint offset);

private:
    std::string     m_name;
    gl::GLint       m_size;
    gl::GLenum      m_type;
    gl::GLuint      m_offset;
};

}

