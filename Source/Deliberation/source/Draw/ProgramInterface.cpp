#include <Deliberation/Draw/ProgramInterface.h>

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/DataLayoutField.h>
#include <Deliberation/Core/StringUtils.h>

#include "Deliberation/Draw/GL/GLType.h"

#include "GL/GLUtils.h"

namespace deliberation
{

ProgramInterface::ProgramInterface() = default;

ProgramInterface::ProgramInterface(gl::GLuint glProgramName)
{
    // Vertex attributes
    {
        auto numAttributes = GLGetProgram(glProgramName, gl::GL_ACTIVE_ATTRIBUTES);
        auto maxNameLength = GLGetProgram(glProgramName, gl::GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);

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
            gl::glGetActiveAttrib(glProgramName, a, namev.size(), &length, &size, &type, namev.data());
            name = std::string(namev.data(), length);

            if (StringStartsWith(name, "gl_"))
            {
                continue;
            }

            location = gl::glGetAttribLocation(glProgramName, name.c_str());

            if (location >= (gl::GLint)m_attributeIndexByLocation.size()) // TODO: Just map location -> uniform, no need for index->
            {
                m_attributeIndexByLocation.resize(location + 1, (unsigned int)-1);
            }

            m_attributeIndexByLocation[location] = i;
            m_attributeIndexByName[name] = i;
            m_attributes.push_back({name, location, type, size});

            i++;
        }
    }

    // Shared between Uniform and UniformBlock inspectors
    std::vector<gl::GLint> uniformOffsets;
    std::unordered_map<gl::GLuint, ProgramInterfaceUniform> blockUniformsByIndex;

    // Uniforms & samplers
    {
        auto numUniforms = GLGetProgram(glProgramName, gl::GL_ACTIVE_UNIFORMS);

        std::vector<gl::GLuint> uniformIndices(numUniforms);
        for (auto u = 0; u < numUniforms; u++)
        {
            uniformIndices[u] = u;
        }

        std::vector<gl::GLint> uniformTypes(numUniforms);
        std::vector<gl::GLint> uniformSizes(numUniforms);
        //std::vector<gl::GLuint> uniformNameLengths(numUniforms);
        std::vector<gl::GLint> uniformBlockIndex(numUniforms);

        uniformOffsets.resize(numUniforms);

        gl::glGetActiveUniformsiv(glProgramName, numUniforms, uniformIndices.data(), gl::GL_UNIFORM_TYPE, uniformTypes.data());
        gl::glGetActiveUniformsiv(glProgramName, numUniforms, uniformIndices.data(), gl::GL_UNIFORM_SIZE, uniformSizes.data());
        gl::glGetActiveUniformsiv(glProgramName, numUniforms, uniformIndices.data(), gl::GL_UNIFORM_OFFSET, uniformOffsets.data());
        //gl::glGetActiveUniformsiv(numUniforms, uniformIndices.data(), gl::GL_UNIFORM_NAME_LENGTH, uniformNameLength.data());
        gl::glGetActiveUniformsiv(glProgramName, numUniforms, uniformIndices.data(), gl::GL_UNIFORM_BLOCK_INDEX, uniformBlockIndex.data());

        m_uniforms.reserve(numUniforms);

        for (auto u = 0; u < numUniforms; u++)
        {
            auto name = GLGetActiveUniformName(glProgramName, u);
            auto size = (gl::GLuint)uniformSizes[u];
            auto location = gl::glGetUniformLocation(glProgramName, name.c_str());
            auto type = (gl::GLenum)uniformTypes[u];

            auto isSampler = false;
            switch (type)
            {
                case gl::GL_SAMPLER_1D: case gl::GL_SAMPLER_2D: case gl::GL_SAMPLER_3D: case gl::GL_SAMPLER_CUBE:
                case gl::GL_SAMPLER_1D_SHADOW: case gl::GL_SAMPLER_2D_SHADOW: case gl::GL_SAMPLER_2D_RECT:
                case gl::GL_SAMPLER_2D_RECT_SHADOW:
                    isSampler = true;
                    break;
                default:
                    isSampler = false;
                    break;
            }

            if (isSampler)
            {
                Assert(location >= 0, "");

                if (location >= (gl::GLint)m_samplerIndexByLocation.size()) // TODO: Just map location -> uniform, no need for index->
                {
                    m_samplerIndexByLocation.resize(location + 1, (unsigned int)-1);
                }

                if (uniformBlockIndex[u] >= -1)
                {
                    std::cout << "Sample '" << name << "' in block, not supported" << std::endl;
                }
                else
                {
                    m_samplerIndexByLocation[location] = m_samplers.size();
                    m_samplerIndexByName[name] = m_samplers.size();
                    m_samplers.emplace_back(name, type, location, (gl::GLuint)uniformSizes[u]);
                }
            }
            else
            {
                if (location >= (gl::GLint)m_uniformIndexByLocation.size()) // TODO: Just map location -> uniform, no need for index->
                {
                    m_uniformIndexByLocation.resize(location + 1, (unsigned int)-1);
                }

                if (size > 1)
                {
                    StringRErase(name, "[0]");
                }

                if (uniformBlockIndex[u] >= 0)
                {
                    blockUniformsByIndex.emplace(std::make_pair(u, ProgramInterfaceUniform(name, GLTypeToType(type), location, size)));
                }
                else
                {
                    Assert(location >= 0, "");

                    m_uniformIndexByLocation[location] = m_uniforms.size();
                    m_uniformIndexByName[name] = m_uniforms.size();
                    m_uniforms.emplace_back(name, GLTypeToType(type), location, size);
                }
            }
        }
    }

    // Uniform Blocks
    {
        auto numUniformBlocks = GLGetProgram(glProgramName, gl::GL_ACTIVE_UNIFORM_BLOCKS);

        for (auto b = 0; b < numUniformBlocks; b++)
        {
            std::vector<DataLayoutField> fields;

            auto name = GLGetActiveUniformBlockName(glProgramName, b);
            auto size = GLGetActiveUniformBlockInt(glProgramName, b, gl::GL_UNIFORM_BLOCK_DATA_SIZE);
            auto numUniforms = GLGetActiveUniformBlockInt(glProgramName, b, gl::GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS);

            Assert(numUniforms >= 0, "");

            std::vector<gl::GLint> indices(numUniforms);
            if (numUniforms > 0)
            {
                gl::glGetActiveUniformBlockiv(glProgramName, b, gl::GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices.data());

                for (auto i = 0; i < indices.size(); i++)
                {
                    auto it = blockUniformsByIndex.find(indices[i]);
                    Assert(it != blockUniformsByIndex.end(), "");

                    auto & uniform = it->second;
                    auto & blockAndName = uniform.name();
                    auto dotPos = blockAndName.rfind('.');
                    std::string strippedName;

                    if (dotPos == std::string::npos)
                    {
                        strippedName = blockAndName;
                    }
                    else
                    {
                        strippedName = blockAndName.substr(dotPos + 1, std::string::npos);
                    }

                    std::cout << "Name: " << strippedName << std::endl;

                    fields.emplace_back(strippedName, uniform.type(), uniformOffsets[indices[i]]);
                }

                m_uniformBlocks.emplace_back(name, DataLayout(std::move(fields), size), b);
                m_uniformBlockByName[name] = m_uniformBlocks.size() - 1;
            }
            else
            {
                std::cout << "Skipping empty uniform block '" << name << "'" << std::endl;
            }
        }

    }

    // Fragment output
    {
        auto activeResources = gl::GLint{};
        auto maxNameLength = gl::GLint{};

        gl::glGetProgramInterfaceiv(glProgramName, gl::GL_PROGRAM_OUTPUT, gl::GL_ACTIVE_RESOURCES, &activeResources);
        gl::glGetProgramInterfaceiv(glProgramName, gl::GL_PROGRAM_OUTPUT, gl::GL_MAX_NAME_LENGTH, &maxNameLength);

        std::vector<char> nameData(maxNameLength);

        for (auto r = 0; r < activeResources; r++)
        {
            auto props =  std::vector<gl::GLenum>({gl::GL_NAME_LENGTH, gl::GL_TYPE, gl::GL_ARRAY_SIZE, gl::GL_LOCATION, gl::GL_LOCATION_INDEX});
            auto params = std::vector<gl::GLint>( {gl::GLint{},        gl::GLint{}, gl::GLint{},       gl::GLint{},     gl::GLint{}});

            gl::glGetProgramResourceiv(glProgramName, gl::GL_PROGRAM_OUTPUT, r, props.size(), props.data(), params.size(), nullptr, params.data());

            auto nameLength = params[0];
            Assert(nameLength > 1, "Program output " + std::to_string(r) + " has no characters in its name. That shouldn't be the case");

            auto type = gl::GLenum(params[1]);
            auto location = params[3];
            Assert(location >= 0, "Program output " + std::to_string(r) + " has a location < 0. That shouldn't be the case");

            auto arraySize = params[2];
            auto locationIndex = params[4];

            gl::glGetProgramResourceName(glProgramName, gl::GL_PROGRAM_OUTPUT, r, nameData.size(), nullptr, nameData.data());

            std::string name(nameData.data(), nameLength - 1);

            if (location >= (gl::GLint)m_fragmentOutputIndexByLocation.size()) // TODO: Just map location -> uniform, no need for index->
            {
                m_fragmentOutputIndexByLocation.resize(location + 1, (unsigned int)-1);
            }

            m_fragmentOutputIndexByLocation[location] = m_fragmentOutputs.size();
            m_fragmentOutputIndexByName[name] = m_fragmentOutputs.size();
            m_fragmentOutputs.push_back({name, type, (unsigned int)location});
        }
    }
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

const ProgramInterfaceSampler & ProgramInterface::sampler(const std::string & name) const
{
    auto iter = m_samplerIndexByName.find(name);
    Assert (iter != m_samplerIndexByName.end(), "No such sampler '" + name + "'");

    return m_samplers[iter->second];
}

const ProgramInterfaceFragmentOutput & ProgramInterface::fragmentOutput(const std::string & name) const
{
    auto iter = m_fragmentOutputIndexByName.find(name);
    Assert (iter != m_fragmentOutputIndexByName.end(), "No such fragmentOutput '" + name + "'");

    return m_fragmentOutputs[iter->second];
}

const ProgramInterfaceUniformBlock & ProgramInterface::uniformBlock(const std::string & name) const
{
    auto iter = m_uniformBlockByName.find(name);
    Assert (iter != m_uniformBlockByName.end(), "No such uniform block '" + name + "'");

    return m_uniformBlocks[iter->second];
}

const ProgramInterfaceVertexAttribute * ProgramInterface::attributeByLocation(unsigned int location) const
{
    if (location >= m_attributeIndexByLocation.size())
    {
        return nullptr;
    }

    auto index = m_attributeIndexByLocation[location];
    if (index == (unsigned int)-1)
    {
        return nullptr;
    }

    return &m_attributes[index];
}

const ProgramInterfaceUniform * ProgramInterface::uniformByLocation(unsigned int location) const
{
    if (location >= m_uniformIndexByLocation.size())
    {
        return nullptr;
    }

    auto index = m_uniformIndexByLocation[location];
    if (index == (unsigned int)-1)
    {
        return nullptr;
    }

    return &m_uniforms[index];
}

const ProgramInterfaceSampler * ProgramInterface::samplerByLocation(unsigned int location) const
{
    if (location >= m_samplerIndexByLocation.size())
    {
        return nullptr;
    }
    auto index = m_samplerIndexByLocation[location];
    if (index == (unsigned int)-1)
    {
        return nullptr;
    }

    return &m_samplers[index];
}

const ProgramInterfaceFragmentOutput * ProgramInterface::fragmentOutputByLocation(unsigned int location) const
{
    if (location >= m_fragmentOutputIndexByLocation.size())
    {
        return nullptr;
    }

    auto index = m_fragmentOutputIndexByLocation[location];
    if (index == (unsigned int)-1)
    {
        return nullptr;
    }

    return &m_fragmentOutputs[index];
}

const std::vector<ProgramInterfaceVertexAttribute> & ProgramInterface::attributes() const
{
    return m_attributes;
}

const std::vector<ProgramInterfaceUniform> & ProgramInterface::uniforms() const
{
    return m_uniforms;
}

const std::vector<ProgramInterfaceSampler> & ProgramInterface::samplers() const
{
    return m_samplers;
}

const std::vector<ProgramInterfaceFragmentOutput> & ProgramInterface::fragmentOutputs() const
{
    return m_fragmentOutputs;
}

const std::vector<ProgramInterfaceUniformBlock> & ProgramInterface::uniformBlocks() const
{
    return m_uniformBlocks;
}

bool ProgramInterface::hasAttribute(const std::string & name) const
{
    auto iter = m_attributeIndexByName.find(name);
    return iter != m_attributeIndexByName.end();
}

bool ProgramInterface::hasUniform(const std::string & name) const
{
    auto iter = m_uniformIndexByName.find(name);
    return iter != m_uniformIndexByName.end();
}

bool ProgramInterface::hasSampler(const std::string & name) const
{
    auto iter = m_samplerIndexByName.find(name);
    return iter != m_samplerIndexByName.end();
}

bool ProgramInterface::hasFragmentOutput(const std::string & name) const
{
    auto iter = m_fragmentOutputIndexByName.find(name);
    return iter != m_fragmentOutputIndexByName.end();
}

bool ProgramInterface::hasUniformBlock(const std::string & name) const
{
    auto iter = m_uniformBlockByName.find(name);
    return iter != m_uniformBlockByName.end();
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

    if (m_samplers.size() != other.m_samplers.size())
    {
        return false;
    }

    if (m_fragmentOutputs.size() != other.m_fragmentOutputs.size())
    {
        return false;
    }

    if (m_uniformBlocks.size() != other.m_uniformBlocks.size())
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

    for (auto s = 0u; s < m_samplers.size(); s++)
    {
        if (m_samplers[s] != other.m_samplers[s])
        {
            return false;
        }
    }

    for (auto fo = 0u; fo < m_fragmentOutputs.size(); fo++)
    {
        if (m_fragmentOutputs[fo] != other.m_fragmentOutputs[fo])
        {
            return false;
        }
    }

    for (auto b = 0u; b < m_samplers.size(); b++)
    {
        if (m_uniformBlocks[b] != other.m_uniformBlocks[b])
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

    stream << "Samplers: " << m_samplers.size() << std::endl;
    for (auto & sampler : m_samplers)
    {
        stream << "  " << sampler.toString() << std::endl;
    }

    stream << "Fragment outputs: " << m_fragmentOutputs.size() << std::endl;
    for (auto & output : m_fragmentOutputs)
    {
        stream << "  " << output.toString() << std::endl;
    }
    return stream.str();
}

}

