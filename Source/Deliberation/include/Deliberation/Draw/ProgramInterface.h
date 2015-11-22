#pragma once

#include <unordered_map>
#include <vector>

#include <Deliberation/Draw/ProgramInterfaceFragmentOutput.h>
#include <Deliberation/Draw/ProgramInterfaceUniform.h>
#include <Deliberation/Draw/ProgramInterfaceVertexAttribute.h>

namespace globjects
{
    class Program;
}

namespace deliberation
{

class ProgramInterface final
{
public:
    ProgramInterface();
    ProgramInterface(const globjects::Program & program);

    const ProgramInterfaceVertexAttribute & attribute(const std::string & name) const;
    const ProgramInterfaceUniform & uniform(const std::string & name) const;

    const ProgramInterfaceVertexAttribute & attributeByLocation(unsigned int location) const;
    const ProgramInterfaceUniform & uniformByLocation(unsigned int location) const;

    const std::vector<ProgramInterfaceVertexAttribute> & attributes() const;
    const std::vector<ProgramInterfaceUniform> & uniforms() const;
    const std::vector<ProgramInterfaceFragmentOutput> & fragmentOutputs() const;

    bool hasAttribute(const std::string & name);
    bool hasUniform(const std::string & name);

    void addAttribute(const ProgramInterfaceVertexAttribute & field);

    bool operator==(const ProgramInterface & other) const;
    bool operator!=(const ProgramInterface & other) const;

    std::string toString() const;

private:
    std::vector<ProgramInterfaceVertexAttribute> m_attributes;
    std::unordered_map<std::string, decltype(m_attributes)::size_type> m_attributeIndexByName;
    std::vector<unsigned int> m_attributeIndexByLocation;

    std::vector<ProgramInterfaceUniform> m_uniforms;
    std::unordered_map<std::string, decltype(m_uniforms)::size_type> m_uniformIndexByName;
    std::vector<unsigned int> m_uniformIndexByLocation;

    std::vector<ProgramInterfaceFragmentOutput> m_fragmentOutputs;
};

}



