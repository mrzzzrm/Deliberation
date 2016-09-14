#pragma once

#include <Deliberation/Core/DataLayout.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API ProgramInterfaceUniformBlock final
{
public:
    ProgramInterfaceUniformBlock(const std::string & name, const DataLayout & layout, unsigned int index);

    const std::string & name() const;
    const DataLayout & layout() const;
    unsigned int index() const;

    bool operator==(const ProgramInterfaceUniformBlock & other) const;
    bool operator!=(const ProgramInterfaceUniformBlock & other) const;

private:
    std::string     m_name;
    DataLayout      m_layout;
    unsigned int    m_index = 0;
};

}