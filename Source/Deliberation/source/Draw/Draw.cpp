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
#include "Detail/ProgramImpl.h"
#include "Detail/UniformBufferBinding.h"

#include "GL/GLVertexAttributeBinder.h"

namespace deliberation
{

Draw::Draw():
    m_impl(nullptr)
{

}

const std::string & Draw::name() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return m_impl->name;
}

Program Draw::program() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return Program(m_impl->program);
}

DrawState & Draw::state()
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return m_impl->state;
}

const DrawState & Draw::state() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return m_impl->state;
}

Framebuffer & Draw::framebuffer()
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return m_impl->framebuffer;
}

const Framebuffer & Draw::framebuffer() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return m_impl->framebuffer;
}

Uniform Draw::uniform(const std::string & name)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto location = m_impl->program->interface.uniform(name).location();

    /*
        TODO
            O(n) complexity? You can do better!
    */

    for (auto & uniform : m_impl->uniforms)
    {
        if (uniform.location == location)
        {
            return Uniform(uniform);
        }
    }

    Fail("No such uniform");
}

Sampler Draw::sampler(const std::string & name)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto location = m_impl->program->interface.sampler(name).location();

    /*
        TODO
            O(n) complexity? You can do better!
    */

    for (auto & sampler : m_impl->samplers)
    {
        if (sampler.location == location)
        {
            return Sampler(sampler);
        }
    }

    Fail("sampler");
}

Buffer Draw::setIndices(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & context = m_impl->context;
    auto buffer = context.createBuffer(data.layout());
    buffer.scheduleUpload(data);
    setIndexBuffer(buffer);

    return buffer;
}

Buffer Draw::addVertices(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & context = m_impl->context;
    auto buffer = context.createBuffer(data.layout());
    buffer.scheduleUpload(data);
    addVertexBuffer(buffer);

    return buffer;
}

Buffer Draw::addInstances(const LayoutedBlob & data, unsigned int divisor)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & context = m_impl->context;
    auto buffer = context.createBuffer(data.layout());
    buffer.scheduleUpload(data);
    addInstanceBuffer(buffer, divisor);

    return buffer;
}

void Draw::setIndexBuffer(const Buffer & buffer)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    Assert(!m_impl->indexBuffer, "Re-setting index buffer not supported (yet)");

    m_impl->indexBuffer = buffer.m_impl;
}

void Draw::addVertexBuffer(const Buffer & buffer)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    verifyVertexBuffer(buffer);
    m_impl->vertexBuffers.push_back(detail::BufferBinding{buffer.m_impl, false, 0u, 0u, 0u});
}

void Draw::addVertexBufferRange(const Buffer & buffer, unsigned int first, unsigned int count)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    verifyVertexBuffer(buffer);
    m_impl->vertexBuffers.push_back(detail::BufferBinding{buffer.m_impl, true, first, count, 0u});
}

void Draw::addInstanceBuffer(const Buffer & buffer, unsigned int divisor)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    verifyInstanceBuffer(buffer);
    m_impl->instanceBuffers.push_back(detail::BufferBinding{buffer.m_impl, false, 0u, 0u, divisor});
}

void Draw::addInstanceBufferRange(const Buffer & buffer, unsigned int first, unsigned int count, unsigned int divisor)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    verifyInstanceBuffer(buffer);
    m_impl->instanceBuffers.push_back(detail::BufferBinding{buffer.m_impl, true, first, count, divisor});
}

void Draw::setFramebuffer(const Framebuffer & framebuffer)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    m_impl->framebuffer = framebuffer;
}

void Draw::setRenderTarget(unsigned int index, Surface * surface)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    m_impl->framebuffer.setRenderTarget(index, surface);
}

void Draw::setRenderTarget(const std::string & name, Surface * surface)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto index = m_impl->program->interface.fragmentOutput(name).location();
    m_impl->framebuffer.setRenderTarget(index, surface);
}

void Draw::setUniformBuffer(const std::string & name, const Buffer & buffer, unsigned int begin)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto index = m_impl->program->interface.uniformBlock(name).index();

    detail::UniformBufferBinding binding{buffer.m_impl, begin};

    m_impl->uniformBuffers[index].reset(binding);
}

void Draw::schedule()
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    if (!isBuild())
    {
        build();
    }

    m_impl->context.scheduleDraw(*this);
}

void Draw::setState(const DrawState & state)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    m_impl->state = state;
}

std::string Draw::toString() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

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
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    return m_impl->glVertexArray != 0u;
}

bool Draw::isComplete() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    if (isBuild())
    {
        return true;
    }

    for (auto & attribute : m_impl->program->interface.attributes())
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
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    Assert(isComplete(), statusString());

    /*
        ToDo
            Move all gl-stuff to Context/GLStateManager
    */

    gl::glGenVertexArrays(1, &m_impl->glVertexArray);
    Assert(m_impl->glVertexArray != 0u, "");

    for (auto & programField : m_impl->program->interface.attributes())
    {
        auto count = 0u;
        auto binding = (detail::BufferBinding*)nullptr;
        auto divisor = 0u;

        auto bufferField = this->bufferField(programField.name(), &binding, &divisor, &count);
        Assert(count == 1u, "");
        Assert(binding, "");

        auto baseoffset = binding->ranged ? binding->first * binding->buffer->layout.stride() : 0;

        GLVertexAttributeBinder binder(m_impl->glVertexArray, m_impl->program->interface, *binding->buffer, divisor);
        binder.bind(bufferField->name(), baseoffset);
    }

    if (m_impl->indexBuffer)
    {
        gl::glBindVertexArray(m_impl->glVertexArray);
        gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_impl->indexBuffer->glName);
    }
}

void Draw::verifyVertexBuffer(const Buffer & buffer) const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    {
        auto iter = std::find_if(m_impl->vertexBuffers.begin(), m_impl->vertexBuffers.end(),
                                 [&buffer] (const detail::BufferBinding & other) { return other.buffer.get() == buffer.m_impl.get(); });
        Assert(iter == m_impl->vertexBuffers.end(), "Duplicate buffer");
    }

    Assert(
        !isComplete(),
        "Can't add vertex buffer, is already complete.\n"
        "Buffer: " + buffer.toString() + "\n"
        "ProgramInterface: " + m_impl->program->interface.toString()
    );
}

void Draw::verifyInstanceBuffer(const Buffer & buffer) const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    {
        auto iter = std::find_if(m_impl->instanceBuffers.begin(), m_impl->instanceBuffers.end(),
                                 [&buffer] (const detail::BufferBinding & other) { return other.buffer.get() == buffer.m_impl.get(); });
        Assert(iter == m_impl->instanceBuffers.end(), "Duplicate buffer");
    }

    Assert(
        !isComplete(),
        "Can't add instance buffer, VAO is already complete.\n"
        "Buffer: " + buffer.toString()
    );
}

const DataLayoutField * Draw::bufferField(const std::string & name,
                                                     detail::BufferBinding ** o_binding,
                                                     gl::GLuint * o_divisor,
                                                     unsigned int * o_count) const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    if (o_count)
    {
        *o_count = 0u;
    }

    auto * field = (const DataLayoutField*)nullptr;

    for (auto b = 0u; b < m_impl->vertexBuffers.size() + m_impl->instanceBuffers.size(); b++)
    {
        auto & binding =  b < m_impl->vertexBuffers.size() ?
                            m_impl->vertexBuffers[b] :
                            m_impl->instanceBuffers[b - m_impl->vertexBuffers.size()];

        auto & buffer = *binding.buffer;

        for (auto & bufferField : buffer.layout.fields())
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
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    std::stringstream stream;

    if (isBuild())
    {
        stream << "VertexArray is build (" << m_impl->program->interface.attributes().size() << " attributes), glName = " << m_impl->glVertexArray << std::endl;
    }
    else
    {
        stream << "VertexArray is not build (" << m_impl->program->interface.attributes().size() << " attributes)" << std::endl;
    }

    for (auto & attribute :  m_impl->program->interface.attributes())
    {
        stream << "  " << attribute.name() << std::endl;
    }

    for (auto & programField :  m_impl->program->interface.attributes())
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

