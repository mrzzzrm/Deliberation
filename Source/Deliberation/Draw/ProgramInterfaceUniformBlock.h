#pragma once

#include <Deliberation/Core/DataLayout.h>

namespace deliberation
{

enum class UniformBufferType
{
    UniformBlock,
    BufferTexture
};

class ProgramInterfaceUniformBlock final
{
  public:
    ProgramInterfaceUniformBlock(
        const std::string & name,
        UniformBufferType type,
        const DataLayout &  layout,
        unsigned int        index);

    const std::string & name() const { return m_name; }
    UniformBufferType   type() const { return m_type; }
    const DataLayout &  layout() const { return m_layout; }
    unsigned int        index() const { return m_index; }

    std::string toString() const;

    bool operator==(const ProgramInterfaceUniformBlock & other) const;
    bool operator!=(const ProgramInterfaceUniformBlock & other) const;

  private:
    std::string         m_name;
    UniformBufferType   m_type;
    DataLayout          m_layout;
    unsigned int        m_index = 0;
};
}