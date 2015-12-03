#include <Deliberation/Draw/ProgramInterfaceSampler.h>

#include <sstream>

namespace deliberation
{

ProgramInterfaceSampler::ProgramInterfaceSampler(const std::string & name,
                                                 gl::GLenum type,
                                                 gl::GLint location,
                                                 gl::GLuint size):
    m_name(name),
    m_type(type),
    m_location(location),
    m_size(size)
{
}

const std::string & ProgramInterfaceSampler::name() const
{
    return m_name;
}

gl::GLenum ProgramInterfaceSampler::type() const
{
    return m_type;
}

gl::GLint ProgramInterfaceSampler::location() const
{
    return m_location;
}

gl::GLuint ProgramInterfaceSampler::size() const
{
    return m_size;
}

std::string ProgramInterfaceSampler::toString() const
{
    std::stringstream stream;
    stream << m_type << " <" << m_name << "> (Size: " << m_size << "; Location: " << m_location << ")";
    return stream.str();
}

bool ProgramInterfaceSampler::operator==(const ProgramInterfaceSampler & other) const
{
    return m_name == other.m_name &&
           m_type == other.m_type &&
           m_location == other.m_location &&
           m_size == other.m_size;
}

bool ProgramInterfaceSampler::operator!=(const ProgramInterfaceSampler & other) const
{
    return !(*this == other);
}

}

