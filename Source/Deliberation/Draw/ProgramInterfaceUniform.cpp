#include <Deliberation/Draw/ProgramInterfaceUniform.h>

#include <sstream>

namespace deliberation
{

ProgramInterfaceUniform::ProgramInterfaceUniform(const std::string & name,
                                                 Type type,
                                                 gl::GLint location,
                                                 u32 arraySize):
    m_name(name),
    m_type(type),
    m_location(location),
    m_arraySize(arraySize)
{
}

const std::string & ProgramInterfaceUniform::name() const
{
    return m_name;
}

Type ProgramInterfaceUniform::type() const
{
    return m_type;
}

gl::GLint ProgramInterfaceUniform::location() const
{
    return m_location;
}

u32 ProgramInterfaceUniform::arraySize() const
{
    return m_arraySize;
}

bool ProgramInterfaceUniform::isArray() const
{
    return m_arraySize > 0;
}

std::string ProgramInterfaceUniform::toString() const
{
    std::stringstream stream;
    stream << m_type.name() << " <" << m_name << "> (Size: " << m_arraySize << "; Location: " << m_location <<
              "; ArraySize: " << m_arraySize << ")";
    return stream.str();
}

bool ProgramInterfaceUniform::operator==(const ProgramInterfaceUniform & other) const
{
    return m_name == other.m_name &&
           m_type == other.m_type &&
           m_location == other.m_location &&
           m_arraySize == other.m_arraySize;
}

bool ProgramInterfaceUniform::operator!=(const ProgramInterfaceUniform & other) const
{
    return !(*this == other);
}

}

