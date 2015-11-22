#include <Deliberation/Draw/Draw.h>

#include <iostream>
#include <sstream>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include "Detail/BufferImpl.h"
#include "Detail/DrawImpl.h"

#include "GL/GLVertexAttributeBinder.h"

namespace deliberation
{

const std::string & Draw::name() const
{
    return m_impl->name;
}

//bool Draw::hasOutput() const
//{
//    return m_output.engaged();
//}
//
//const DrawOutputConfig & Draw::output() const
//{
//    Assert(hasOutput(), "");
//    return m_output.get();
//}
//
DrawState & Draw::state()
{
    return m_impl->state;
}

const DrawState & Draw::state() const
{
    return m_impl->state;
}

void Draw::setIndexBuffer(const Buffer & buffer)
{
    Assert(!m_impl->indexBuffer, "Re-setting index buffer not supported (yet)");

    m_impl->indexBuffer = &buffer;
}

void Draw::addVertexBuffer(const Buffer & buffer)
{
    verifyVertexBuffer(buffer);
    m_impl->vertexBuffers.push_back(detail::BufferBinding{std::cref(buffer), false, 0u, 0u, 0u});
}

void Draw::addVertexBufferRange(const Buffer & buffer, unsigned int first, unsigned int count)
{
    verifyVertexBuffer(buffer);
    m_impl->vertexBuffers.push_back(detail::BufferBinding{std::cref(buffer), true, first, count, 0u});
}

void Draw::addInstanceBuffer(const Buffer & buffer, unsigned int divisor)
{
    verifyInstanceBuffer(buffer);
    m_impl->instanceBuffers.push_back(detail::BufferBinding{std::cref(buffer), false, 0u, 0u, divisor});
}

void Draw::addInstanceBufferRange(const Buffer & buffer, unsigned int first, unsigned int count, unsigned int divisor)
{
    verifyInstanceBuffer(buffer);
    m_impl->instanceBuffers.push_back(detail::BufferBinding{std::cref(buffer), true, first, count, divisor});
}

void Draw::schedule()
{
    if (!isBuild())
    {
        build();
    }

    m_impl->context.scheduleDraw(*this);
}


//void Draw::setTexture(const std::string & name, const globjects::Texture & texture)
//{
//    auto & uniform = m_program.layout().uniform(name);
//
//    Assert(gladv::GLSLType(uniform.type()).isSampler(), "Uniform " + uniform.name() + " is no sampler");
//
//    auto location = uniform.location();
//
//    decltype(m_textureUnitCounter) unit;
//
//    auto i = m_textureUnitByLocation.find(location);
//    if (i == m_textureUnitByLocation.end())
//    {
//        unit = m_textureUnitBindings.size();
//        m_textureUnitBindings.push_back(TextureUnitBinding{&texture, nullptr});
//        m_textureUnitByLocation[location] = unit;
//    }
//    else
//    {
//        unit = i->second;
//        m_textureUnitBindings[unit].texture = &texture;
//    }
//
//    if ((int)m_uniforms.size() <= location)
//    {
//        m_uniforms.resize(location + 1);
//    }
//
//    // Sampler uniform has to be set to an int, not a uint
//    m_uniforms[location] = gladv::GLSLValue((int)unit);
//
//    d_unsetUniformLocations.erase(location);
//}
//
//void Draw::setSampler(const std::string & name, const globjects::Sampler & sampler)
//{
//    auto & uniform = m_program.layout().uniform(name);
//
//    Assert(gladv::GLSLType(uniform.type()).isSampler(), "Uniform " + uniform.name() + " is no sampler");
//
//    auto location = uniform.location();
//
//    decltype(m_textureUnitCounter) unit;
//
//    auto i = m_textureUnitByLocation.find(location);
//    if (i == m_textureUnitByLocation.end())
//    {
//        unit = m_textureUnitBindings.size();
//        m_textureUnitBindings.push_back(TextureUnitBinding{nullptr, &sampler});
//        m_textureUnitByLocation[location] = unit;
//    }
//    else
//    {
//        unit = i->second;
//        m_textureUnitBindings[unit].sampler = &sampler;
//    }
//
//    if ((int)m_uniforms.size() <= location)
//    {
//        m_uniforms.resize(location + 1);
//    }
//
//    // Sampler uniform has to be set to an int, not a uint
//    m_uniforms[location] = gladv::GLSLValue((int)unit);
//
//    d_unsetUniformLocations.erase(location);
//}
//
//void Draw::setOutput(const DrawOutputConfig & output)
//{
//    m_output.reset(output);
//}

void Draw::setState(const DrawState & state)
{
    m_impl->state = state;
}

std::string Draw::toString() const
{
    std::stringstream stream;
    stream << statusString() << std::endl;
//    stream << "Draw:" << std::endl;
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

Draw::Draw(const std::shared_ptr<detail::DrawImpl> & impl):
    m_impl(impl)
{

}

bool Draw::isBuild() const
{
    return m_impl->glVertexArray != 0u;
}

bool Draw::isComplete() const
{
    if (isBuild())
    {
        return true;
    }

    for (auto & attribute : m_impl->program.interface().attributes())
    {
        auto count = 0u;
        bufferField(attribute.name(), nullptr, nullptr, &count);

        if (count != 1u)
        {
            return false;
        }
    }

    return true;
}

void Draw::build()
{
    Assert(isComplete(), statusString());

    /*
        ToDo
            Move all gl-stuff to Context/GLStateManager
    */

    gl::glGenVertexArrays(1, &m_impl->glVertexArray);
    Assert(m_impl->glVertexArray != 0u, "");

    for (auto & programField : m_impl->program.interface().attributes())
    {
        auto count = 0u;
        auto binding = (detail::BufferBinding*)nullptr;
        auto divisor = 0u;

        auto bufferField = this->bufferField(programField.name(), &binding, &divisor, &count);
        Assert(count == 1u, "");
        Assert(binding, "");

        auto baseoffset = binding->ranged ? binding->first * binding->buffer.get().layout().stride() : 0;

        GLVertexAttributeBinder binder(m_impl->glVertexArray, m_impl->program.interface(), binding->buffer.get(), divisor);
        binder.bind(bufferField->name(), baseoffset);
    }

    if (m_impl->indexBuffer)
    {
        gl::glBindVertexArray(m_impl->glVertexArray);
        gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_impl->indexBuffer->m_impl->glName);
    }
}

void Draw::verifyVertexBuffer(const Buffer & buffer) const
{
    {
        auto iter = std::find_if(m_impl->vertexBuffers.begin(), m_impl->vertexBuffers.end(),
                                 [&buffer] (const detail::BufferBinding & other) { return &other.buffer.get() == &buffer; });
        Assert(iter == m_impl->vertexBuffers.end(), "Duplicate buffer");
    }

    Assert(
        !isComplete(),
        "Can't add vertex buffer, is already complete.\n"
        "Buffer: " + buffer.toString() + "\n"
        "ProgramInterface: " + m_impl->program.interface().toString()
    );
}

void Draw::verifyInstanceBuffer(const Buffer & buffer) const
{
    {
        auto iter = std::find_if(m_impl->instanceBuffers.begin(), m_impl->instanceBuffers.end(),
                                 [&buffer] (const detail::BufferBinding & other) { return &other.buffer.get() == &buffer; });
        Assert(iter == m_impl->instanceBuffers.end(), "Duplicate buffer");
    }

    Assert(
        !isComplete(),
        "Can't add instance buffer, VAO is already complete.\n"
        "Buffer: " + buffer.toString()
    );
}

const BufferLayoutField * Draw::bufferField(const std::string & name,
                                                     detail::BufferBinding ** o_binding,
                                                     gl::GLuint * o_divisor,
                                                     unsigned int * o_count) const
{
    if (o_count)
    {
        *o_count = 0u;
    }

    auto * field = (const BufferLayoutField*)nullptr;

    for (auto b = 0u; b < m_impl->vertexBuffers.size() + m_impl->instanceBuffers.size(); b++)
    {
        auto & binding =  b < m_impl->vertexBuffers.size() ?
                            m_impl->vertexBuffers[b] :
                            m_impl->instanceBuffers[b - m_impl->vertexBuffers.size()];

        auto & buffer = binding.buffer.get();

        for (auto & bufferField : buffer.layout().fields())
        {
            if (bufferField.name() == name)
            {
                field = &bufferField;

                if (o_divisor)
                {
                    *o_divisor = binding.divisor;
                }

                if (o_binding)
                {
                    *o_binding = &binding;
                }

                if (o_count)
                {
                    (*o_count)++;
                }
            }
        }
    }

    return field;
}

std::string Draw::statusString() const
{
  std::stringstream stream;

    if (isBuild())
    {
        stream << "VertexArray is build (" << m_impl->program.interface().attributes().size() << " attributes), glName = " << m_impl->glVertexArray << std::endl;
    }
    else
    {
        stream << "VertexArray is not build (" << m_impl->program.interface().attributes().size() << " attributes)" << std::endl;
    }

    for (auto & attribute :  m_impl->program.interface().attributes())
    {
        stream << "  " << attribute.name() << std::endl;
    }

    for (auto & programField :  m_impl->program.interface().attributes())
    {
        auto count = 0u;

        bufferField(programField.name(), nullptr, nullptr, &count);

        if (count > 1u)
        {
            stream << "Draw is supplied with multiple attributes '" << programField.name() << "'";
            return stream.str();
        }

        if (count == 0u)
        {
            stream << "Draw isn't supplied with attribute '" << programField.name() << "'";
            return stream.str();
        }
    }

    return stream.str();
}

}

