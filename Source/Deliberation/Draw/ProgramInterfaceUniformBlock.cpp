#include <Deliberation/Draw/ProgramInterfaceUniformBlock.h>

#include <sstream>

namespace deliberation
{
ProgramInterfaceUniformBlock::ProgramInterfaceUniformBlock(
    const std::string & name, const DataLayout & layout, unsigned int index)
    : m_name(name), m_layout(layout), m_index(index)
{
}

std::string ProgramInterfaceUniformBlock::toString() const
{
    std::stringstream stream;

    static const char * TYPES[] = {"UniformBlock", "TextureBuffer"};

    stream << "{Name: " << m_name << "; Index: " << m_index
           << "; Layout: " << m_layout.toString() << "}";

    return stream.str();
};

bool ProgramInterfaceUniformBlock::
     operator==(const ProgramInterfaceUniformBlock & other) const
{
    return m_name == other.m_name && m_layout.equals(other.m_layout) &&
           m_index == other.m_index;
}

bool ProgramInterfaceUniformBlock::
     operator!=(const ProgramInterfaceUniformBlock & other) const
{
    return !(operator==(other));
}
}