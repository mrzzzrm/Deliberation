#include <Deliberation/Draw/ProgramInterfaceUniformBlock.h>

namespace deliberation
{

ProgramInterfaceUniformBlock::ProgramInterfaceUniformBlock(const std::string & name,
                                                           const DataLayout & layout,
                                                           unsigned int index):
    m_name(name),
    m_layout(layout),
    m_index(index)
{

}

const std::string & ProgramInterfaceUniformBlock::name() const
{
    return m_name;
}

const DataLayout & ProgramInterfaceUniformBlock::layout() const
{
    return m_layout;
}

unsigned int ProgramInterfaceUniformBlock::index() const
{
    return m_index;
}

}