#pragma once

#include <glbinding/gl/enum.h>

#include <string>



namespace deliberation
{

class ProgramInterfaceFragmentOutput final
{
public:
    ProgramInterfaceFragmentOutput(const std::string & name,
                                gl::GLenum type,
                                unsigned int location);

    const std::string & name() const;
    gl::GLenum type() const;
    unsigned int location() const;

    std::string toString() const;

    bool operator==(const ProgramInterfaceFragmentOutput & other) const;
    bool operator!=(const ProgramInterfaceFragmentOutput & other) const;

private:
    std::string     m_name;
    gl::GLenum      m_type;
    unsigned int    m_location;
};

}

