#include <Deliberation/Draw/ProgramInterfaceUniform.h>

#include <sstream>

namespace deliberation
{

ProgramInterfaceUniform::ProgramInterfaceUniform(const std::string & name,
                                                 gl::GLenum type,
                                                 gl::GLint location,
                                                 gl::GLuint size):
    m_name(name),
    m_type(type),
    m_location(location),
    m_size(size)
{
}

const std::string & ProgramInterfaceUniform::name() const
{
    return m_name;
}

gl::GLenum ProgramInterfaceUniform::type() const
{
    return m_type;
}

gl::GLint ProgramInterfaceUniform::location() const
{
    return m_location;
}

gl::GLuint ProgramInterfaceUniform::size() const
{
    return m_size;
}

std::string ProgramInterfaceUniform::toString() const
{
    std::stringstream stream;
    stream << m_type << " <" << m_name << "> (Size: " << m_size << "; Location: " << m_location << ")";
    return stream.str();
}

bool ProgramInterfaceUniform::operator==(const ProgramInterfaceUniform & other) const
{
    return m_name == other.m_name &&
           m_type == other.m_type &&
           m_location == other.m_location &&
           m_size == other.m_size;
}

bool ProgramInterfaceUniform::operator!=(const ProgramInterfaceUniform & other) const
{
    return !(*this == other);
}

}

