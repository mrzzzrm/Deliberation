#include <Deliberation/Draw/Draw.h>

#include <iostream>
#include <sstream>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Buffer.h>

const std::string & DrawCommand::name() const
{
    return m_impl->name();
}

//bool DrawCommand::hasOutput() const
//{
//    return m_output.engaged();
//}
//
//const DrawOutputConfig & DrawCommand::output() const
//{
//    Assert(hasOutput(), "");
//    return m_output.get();
//}

DrawState & DrawCommand::state()
{
    return m_state;
}

const DrawState & DrawCommand::state() const
{
    return m_state;
}

void DrawCommand::setIndexBuffer(const Buffer & buffer)
{
    m_vao.setIndexBuffer(buffer.buffer());
    m_indexBuffer = &buffer;
}

void DrawCommand::addVertexBuffer(const Buffer & buffer)
{
    m_vao.addVertexBuffer(buffer.buffer());
    m_vertexBuffers.push_back(BufferBinding{std::cref(buffer), false, 0u, 0u});
}

void DrawCommand::addVertexBufferRange(const Buffer & buffer, unsigned int first, unsigned int count)
{
    m_vao.addVertexBufferRange(buffer.buffer(), first, count);
    m_vertexBuffers.push_back(BufferBinding{std::cref(buffer), true, first, count});
}

void DrawCommand::addInstanceBuffer(const Buffer & buffer, unsigned int divisor)
{
    m_vao.addInstanceBuffer(buffer.buffer(), divisor);
    m_instanceBuffers.push_back(std::make_pair(std::cref(buffer), divisor));
}

void DrawCommand::setTexture(const std::string & name, const globjects::Texture & texture)
{
    auto & uniform = m_program.layout().uniform(name);

    Assert(gladv::GLSLType(uniform.type()).isSampler(), "Uniform " + uniform.name() + " is no sampler");

    auto location = uniform.location();

    decltype(m_textureUnitCounter) unit;

    auto i = m_textureUnitByLocation.find(location);
    if (i == m_textureUnitByLocation.end())
    {
        unit = m_textureUnitBindings.size();
        m_textureUnitBindings.push_back(TextureUnitBinding{&texture, nullptr});
        m_textureUnitByLocation[location] = unit;
    }
    else
    {
        unit = i->second;
        m_textureUnitBindings[unit].texture = &texture;
    }

    if ((int)m_uniforms.size() <= location)
    {
        m_uniforms.resize(location + 1);
    }

    // Sampler uniform has to be set to an int, not a uint
    m_uniforms[location] = gladv::GLSLValue((int)unit);

    d_unsetUniformLocations.erase(location);
}

void DrawCommand::setSampler(const std::string & name, const globjects::Sampler & sampler)
{
    auto & uniform = m_program.layout().uniform(name);

    Assert(gladv::GLSLType(uniform.type()).isSampler(), "Uniform " + uniform.name() + " is no sampler");

    auto location = uniform.location();

    decltype(m_textureUnitCounter) unit;

    auto i = m_textureUnitByLocation.find(location);
    if (i == m_textureUnitByLocation.end())
    {
        unit = m_textureUnitBindings.size();
        m_textureUnitBindings.push_back(TextureUnitBinding{nullptr, &sampler});
        m_textureUnitByLocation[location] = unit;
    }
    else
    {
        unit = i->second;
        m_textureUnitBindings[unit].sampler = &sampler;
    }

    if ((int)m_uniforms.size() <= location)
    {
        m_uniforms.resize(location + 1);
    }

    // Sampler uniform has to be set to an int, not a uint
    m_uniforms[location] = gladv::GLSLValue((int)unit);

    d_unsetUniformLocations.erase(location);
}

void DrawCommand::setOutput(const DrawOutputConfig & output)
{
    m_output.reset(output);
}

void DrawCommand::setState(const DrawState & state)
{
    m_state = state;
}

std::string DrawCommand::toString() const
{
    std::stringstream stream;
//    stream << "DrawCommand:" << std::endl;
//    stream << "{" << std::endl;
//    stream << "  Program: " << (m_program.toString()) << std::endl;
//    stream << "  State: " << m_state.toString() << std::endl;
//    stream << "  VAO: " << m_vao.toString() << std::endl;
////    stream << "  InstanceCount: " << (m_instanceBuffers.empty() ? 0 : instanceCount()) << std::endl;
////    stream << "  VertexCount: " << vertexCount() << std::endl;
////    stream << "  IndexCount: " << (m_indexBuffer ? elementCount() : 0u) << std::endl;
//    stream << "  InstanceBuffers:" << std::endl;
//    for (auto & pair : m_instanceBuffers)
//    {
//        stream << "    " << &pair.first << ": " << pair.first.get().count() << " instances; Divisor = " << pair.second << "; ID = " << pair.first.get().buffer().id() << std::endl;
//    }
//
//    if (!d_unsetUniformLocations.empty())
//    {
//        stream << "  Unset uniforms: " << std::endl;
//        for (auto location : d_unsetUniformLocations)
//        {
//            stream << "    " << m_program.layout().uniformByLocation(location).toString() << std::endl;
//        }
//    }
//
//    stream << "}" << std::endl;

    return stream.str();
}

DrawCommand::DrawCommand(const std::shared_ptr<detail::DrawImpl> & impl):
    m_impl(impl)
{

}

