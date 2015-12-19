#include <Deliberation/Draw/ProgramInterfaceFragmentOutput.h>

#include <glbinding/Meta.h>

namespace deliberation
{

ProgramInterfaceFragmentOutput::ProgramInterfaceFragmentOutput(const std::string & name,
                                                         gl::GLenum type,
                                                         unsigned int location):
    m_name(name),
    m_type(type),
    m_location(location)
{
}

const std::string & ProgramInterfaceFragmentOutput::name() const
{
    return m_name;
}

gl::GLenum ProgramInterfaceFragmentOutput::type() const
{
    return m_type;
}

unsigned int ProgramInterfaceFragmentOutput::location() const
{
    return m_location;
}

std::string ProgramInterfaceFragmentOutput::toString() const
{
    return "  " + std::to_string(m_location) + ": " + glbinding::Meta::getString(m_type) + " '" + m_name + "'";
}

bool ProgramInterfaceFragmentOutput::operator==(const ProgramInterfaceFragmentOutput & other) const
{
    return m_name == other.m_name && m_type == other.m_type && m_location == other.m_location;
}

bool ProgramInterfaceFragmentOutput::operator!=(const ProgramInterfaceFragmentOutput & other) const
{
    return !operator==(other);
}

}

