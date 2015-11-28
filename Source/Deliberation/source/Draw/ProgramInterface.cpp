#include <Deliberation/Draw/ProgramInterface.h>

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

#include <glbinding/gl/enum.h>

#include <globjects/Program.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StringUtils.h>

namespace deliberation
{

ProgramInterface::ProgramInterface() = default;

ProgramInterface::ProgramInterface(const globjects::Program & program)
{
    // Vertex attributes
    {
        auto numAttributes = program.get(gl::GL_ACTIVE_ATTRIBUTES);
        auto maxNameLength = program.get(gl::GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);

        auto namev = std::vector<gl::GLchar>{};
        namev.resize(maxNameLength);

        auto name = std::string{};

        auto type = gl::GLenum{};
        auto size = gl::GLint{};

        auto location = gl::GLint{};

        auto length = gl::GLint{};

        unsigned int i = 0u;

        for (auto a = 0; a < numAttributes; a++)
        {
            program.getActiveAttrib(a, namev.size(), &length, &size, &type, namev.data());
            name = std::string(namev.data(), length);

            if (StringStartsWith(name, "gl_"))
            {
                continue;
            }

            location = program.getAttributeLocation(name);

            if (location >= (gl::GLint)m_attributeIndexByLocation.size()) // TODO: Just map location -> uniform, no need for index->
            {
                m_attributeIndexByLocation.resize(location + 1, (unsigned int)-1);
                m_attributeIndexByLocation[location] = i;
            }

            m_attributeIndexByName[name] = i;
            m_attributes.push_back({name, location, type, size});

            i++;
        }
    }

    // Uniforms
    {
        auto numUniforms = program.get(gl::GL_ACTIVE_UNIFORMS);

        std::vector<gl::GLuint> uniformIndices(numUniforms);
        for (auto u = 0; u < numUniforms; u++)
        {
            uniformIndices[u] = u;
        }

        std::vector<gl::GLint> uniformTypes(numUniforms);
        std::vector<gl::GLint> uniformSizes(numUniforms);
        //std::vector<gl::GLuint> uniformNameLengths(numUniforms);
        std::vector<gl::GLint> uniformBlockIndex(numUniforms);

        program.getActiveUniforms(numUniforms, uniformIndices.data(), gl::GL_UNIFORM_TYPE, uniformTypes.data());
        program.getActiveUniforms(numUniforms, uniformIndices.data(), gl::GL_UNIFORM_SIZE, uniformSizes.data());
        //program.getActiveUniforms(numUniforms, uniformIndices.data(), gl::GL_UNIFORM_NAME_LENGTH, uniformNameLength.data());
        program.getActiveUniforms(numUniforms, uniformIndices.data(), gl::GL_UNIFORM_BLOCK_INDEX, uniformBlockIndex.data());

        m_uniforms.reserve(numUniforms);

        for (auto u = 0; u < numUniforms; u++)
        {
            if (uniformBlockIndex[u] != -1)
            {
                std::cout << "WARNING: No support for block uniforms in layout" << std::endl;
                continue;
            }

            auto name = program.getActiveUniformName(u);
            auto location = program.getUniformLocation(name.c_str());

            if (location >= (gl::GLint)m_uniformIndexByLocation.size()) // TODO: Just map location -> uniform, no need for index->
            {
                m_uniformIndexByLocation.resize(location + 1, (unsigned int)-1);
            }

            m_uniformIndexByLocation[location] = u;

            m_uniforms.push_back({name, (gl::GLenum)uniformTypes[u], location, (gl::GLuint)uniformSizes[u]});
            m_uniformIndexByName[name] = u;
        }
    }

    // Fragment output
    {
        auto activeResources = gl::GLint{};
        auto maxNameLength = gl::GLint{};

        program.getInterface(gl::GL_PROGRAM_OUTPUT, gl::GL_ACTIVE_RESOURCES, &activeResources);
        program.getInterface(gl::GL_PROGRAM_OUTPUT, gl::GL_MAX_NAME_LENGTH, &maxNameLength);

        std::vector<char> nameData(maxNameLength);

        std::cout << "Active outputs: " << activeResources << std::endl;

        for (auto r = 0; r < activeResources; r++)
        {
            auto props =  std::vector<gl::GLenum>({gl::GL_NAME_LENGTH, gl::GL_TYPE, gl::GL_ARRAY_SIZE, gl::GL_LOCATION, gl::GL_LOCATION_INDEX});
            auto params = std::vector<gl::GLint>( {gl::GLint{},        gl::GLint{}, gl::GLint{},       gl::GLint{},     gl::GLint{}});

            program.getResource(gl::GL_PROGRAM_OUTPUT, r, props.size(), props.data(), params.size(), nullptr, params.data());

            auto nameLength = params[0];
            Assert(nameLength > 1, "Program output " + std::to_string(r) + " has no characters in its name. That shouldn't be the case");

            auto type = gl::GLenum(params[1]);
            auto location = params[3];
            Assert(location >= 0, "Program output " + std::to_string(r) + " has a location < 0. That shouldn't be the case");

            auto arraySize = params[2];
            auto locationIndex = params[4];

            program.getResourceName(gl::GL_PROGRAM_OUTPUT, r, nameData.size(), nullptr, nameData.data());

            std::string name(nameData.data(), nameLength - 1);

            m_fragmentOutputs.push_back({name, type, (unsigned int)location});
        }
    }

    std::cout << "Layout: " << toString() << std::endl;
}

const ProgramInterfaceVertexAttribute & ProgramInterface::attribute(const std::string & name) const
{
    auto iter = m_attributeIndexByName.find(name);
    if (iter == m_attributeIndexByName.end())
    {
        std::cout << "No such attribute '" << name << "'" << std::endl;
        assert(false);
    }

    return m_attributes[iter->second];
}

const ProgramInterfaceUniform & ProgramInterface::uniform(const std::string & name) const
{
    auto iter = m_uniformIndexByName.find(name);
    Assert (iter != m_uniformIndexByName.end(), "No such uniform '" + name + "'");

    return m_uniforms[iter->second];
}

const ProgramInterfaceVertexAttribute & ProgramInterface::attributeByLocation(unsigned int location) const
{
    Assert(location < m_attributeIndexByLocation.size(), "No such attribute location " + std::to_string(location));

    auto index = m_attributeIndexByLocation[location];
    Assert(index != (unsigned int)-1, "No such attribute location " + std::to_string(location));

    return m_attributes[index];
}

const ProgramInterfaceUniform & ProgramInterface::uniformByLocation(unsigned int location) const
{
    Assert(location < m_uniformIndexByLocation.size(), "No such uniform location " + std::to_string(location));

    auto index = m_uniformIndexByLocation[location];
    Assert(index != (unsigned int)-1, "No such uniform location " + std::to_string(location));

    return m_uniforms[index];
}

const std::vector<ProgramInterfaceVertexAttribute> & ProgramInterface::attributes() const
{
    return m_attributes;
}

const std::vector<ProgramInterfaceUniform> & ProgramInterface::uniforms() const
{
    return m_uniforms;
}

const std::vector<ProgramInterfaceFragmentOutput> & ProgramInterface::fragmentOutputs() const
{
    return m_fragmentOutputs;
}

bool ProgramInterface::hasAttribute(const std::string & name)
{
    auto iter = m_attributeIndexByName.find(name);
    return iter != m_attributeIndexByName.end();
}

bool ProgramInterface::hasUniform(const std::string & name)
{
    auto iter = m_uniformIndexByName.find(name);
    return iter != m_uniformIndexByName.end();
}

bool ProgramInterface::operator==(const ProgramInterface & other) const
{
    if (m_attributes.size() != other.m_attributes.size())
    {
        return false;
    }

    if (m_uniforms.size() != other.m_uniforms.size())
    {
        return false;
    }

    for (auto a = 0u; a < m_attributes.size(); a++)
    {
        if (m_attributes[a] != other.m_attributes[a])
        {
            return false;
        }
    }

    for (auto u = 0u; u < m_uniforms.size(); u++)
    {
        if (m_uniforms[u] != other.m_uniforms[u])
        {
            return false;
        }
    }

    return true;
}

bool ProgramInterface::operator!=(const ProgramInterface & other) const
{
    return !(*this == other);
}

std::string ProgramInterface::toString() const
{
    std::stringstream stream;
    stream << "ProgramInterface" << std::endl;
    stream << "Attributes: " << m_attributes.size() << std::endl;
    for (auto & attribute : m_attributes)
    {
        stream << "  " << attribute.toString() << std::endl;
    }

    stream << "Uniforms: " << m_uniforms.size() << std::endl;
    for (auto & uniform : m_uniforms)
    {
        stream << "  " << uniform.toString() << std::endl;
    }

    stream << "Fragment outputs: " << m_fragmentOutputs.size() << std::endl;
    for (auto & output : m_fragmentOutputs)
    {
        stream << "  " << output.toString() << std::endl;
    }
    return stream.str();
}

}

