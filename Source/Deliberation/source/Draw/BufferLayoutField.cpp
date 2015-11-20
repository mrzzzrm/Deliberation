#include <Deliberation/Draw/BufferLayoutField.h>

namespace deliberation
{

BufferLayoutField::BufferLayoutField(const std::string & name,
                                     gl::GLint size,
                                     gl::GLenum type,
                                     gl::GLuint offset):
    m_name(name),
    m_size(size),
    m_type(type),
    m_offset(offset)
{

}

const std::string & BufferLayoutField::name() const
{
    return m_name;
}

gl::GLint BufferLayoutField::size() const
{
    return m_size;
}

gl::GLenum BufferLayoutField::type() const
{
    return m_type;
}

gl::GLuint BufferLayoutField::offset() const
{
    return m_offset;
}

void BufferLayoutField::setName(const std::string & name)
{
    m_name = name;
}

void BufferLayoutField::setSize(gl::GLint size)
{
    m_size = size;
}

void BufferLayoutField::setType(gl::GLenum type)
{
    m_type = type;
}

void BufferLayoutField::setOffset(gl::GLuint offset)
{
    m_offset = offset;
}

}


