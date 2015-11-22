#include <Deliberation/Draw/ProgramInterfaceVertexAttribute.h>

#include <sstream>

#include <glbinding/gl/enum.h>

namespace deliberation
{

ProgramInterfaceVertexAttribute::ProgramInterfaceVertexAttribute():
    m_name(),
    m_location(-1),
    m_type(gl::GL_NONE),
    m_size(0)
{

}

ProgramInterfaceVertexAttribute::ProgramInterfaceVertexAttribute(const std::string & name,
                                                           gl::GLint location,
                                                           gl::GLenum type,
                                                           gl::GLint size):
    m_name(name),
    m_location(location),
    m_type(type),
    m_size(size)
{

}

const std::string & ProgramInterfaceVertexAttribute::name() const
{
    return m_name;
}

gl::GLint ProgramInterfaceVertexAttribute::location() const
{
    return m_location;
}

gl::GLenum ProgramInterfaceVertexAttribute::type() const
{
    return m_type;
}

gl::GLint ProgramInterfaceVertexAttribute::size() const
{
    return m_size;
}

bool ProgramInterfaceVertexAttribute::operator==(const ProgramInterfaceVertexAttribute & other) const
{
    return m_name == other.m_name &&
           m_location == other.m_location &&
           m_type == other.m_type &&
           m_size == other.m_size;
}

bool ProgramInterfaceVertexAttribute::operator!=(const ProgramInterfaceVertexAttribute & other) const
{
    return !(*this == other);
}

std::string ProgramInterfaceVertexAttribute::toString() const
{
    std::stringstream stream;
    stream << m_location << ": " << m_type << " " << m_name << " (size: " << m_size << ")";

    return stream.str();
}

}

