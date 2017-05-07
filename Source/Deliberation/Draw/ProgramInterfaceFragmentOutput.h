#pragma once

#include <glbinding/gl/enum.h>

#include <string>

#include <Deliberation/Core/Types.h>

namespace deliberation
{

class ProgramInterfaceFragmentOutput final
{
public:
    ProgramInterfaceFragmentOutput(
        const std::string & name,
        Type type,
        unsigned int location);

    const std::string & name() const;
    Type type() const;
    unsigned int location() const;

    std::string toString() const;

    bool operator==(const ProgramInterfaceFragmentOutput & other) const;
    bool operator!=(const ProgramInterfaceFragmentOutput & other) const;

private:
    std::string     m_name;
    Type            m_type;
    unsigned int    m_location;
};

}

