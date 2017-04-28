#include <Deliberation/Draw/Draw.h>

#include <algorithm>
#include <iostream>
#include <sstream>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>
#include <Deliberation/Draw/Detail/VertexAttributeBinding.h>

#include "Detail/BufferImpl.h"
#include "Detail/DrawImpl.h"
#include "Detail/ProgramImpl.h"
#include "Detail/UniformBufferBinding.h"

#include "Draw/GL/GLBindVertexAttribute.h"

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

    auto location = m_impl->program->interface.uniformRef(name).location();

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

    auto location = m_impl->program->interface.samplerRef(name).location();

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

    auto & drawContext = m_impl->drawContext;
    auto buffer = drawContext.createBuffer(data.layout());
    buffer.scheduleUpload(data);
    setIndexBuffer(buffer);

    return buffer;
}

Buffer Draw::addVertices(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & drawContext = m_impl->drawContext;
    auto buffer = drawContext.createBuffer(data.layout());
    buffer.scheduleUpload(data);
    addVertexBuffer(buffer);

    return buffer;
}

Buffer Draw::addInstances(const LayoutedBlob & data, u32 divisor)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & drawContext = m_impl->drawContext;
    auto buffer = drawContext.createBuffer(data.layout());
    buffer.scheduleUpload(data);
    addInstanceBuffer(buffer, divisor);

    return buffer;
}

void Draw::setIndexBuffer(const Buffer & buffer)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    Assert(buffer.layout().fields().size() == 1, "Invalid index buffer layout");

    m_impl->indexBufferBinding.buffer = buffer.m_impl;
    m_impl->indexBufferBinding.ranged = false;
    m_impl->indexBufferBindingDirty = true;
}

void Draw::setIndexBufferRange(const Buffer & buffer, u32 first, u32 count)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    Assert(buffer.layout().fields().size() == 1, "Invalid index buffer layout");

    m_impl->indexBufferBinding.buffer = buffer.m_impl;
    m_impl->indexBufferBinding.ranged = true;
    m_impl->indexBufferBinding.first = first;
    m_impl->indexBufferBinding.count = count;
    m_impl->indexBufferBindingDirty = true;
}

void Draw::addVertexBuffer(const Buffer & buffer)
{
    addVertexBuffer(buffer, false, 0, 0, 0);
}

void Draw::addVertexBufferRange(const Buffer & buffer, u32 first, u32 count)
{
    addVertexBuffer(buffer, true, first, count, 0);
}

void Draw::addInstanceBuffer(const Buffer & buffer, u32 divisor)
{
    addVertexBuffer(buffer, false, 0, 0, divisor);
}

void Draw::addInstanceBufferRange(const Buffer & buffer, u32 first, u32 count, u32 divisor)
{
    addVertexBuffer(buffer, true, first, count, divisor);
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

    auto index = m_impl->program->interface.fragmentOutputRef(name).location();
    m_impl->framebuffer.setRenderTarget(index, surface);
}

void Draw::setUniformBuffer(const std::string & name, const Buffer & buffer, unsigned int begin)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto index = m_impl->program->interface.uniformBlockRef(name).index();

    detail::UniformBufferBinding binding{buffer.m_impl, begin};

    m_impl->uniformBuffers[index].reset(binding);
}

void Draw::schedule() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    if (m_impl->glVertexArray == 0) build();

    if (m_impl->indexBufferBindingDirty && m_impl->indexBufferBinding.buffer)
    {
        gl::glBindVertexArray(m_impl->glVertexArray);
        gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_impl->indexBufferBinding.buffer->glName);
        m_impl->indexBufferBindingDirty = false;
    }

    if (!m_impl->dirtyValueAttributes.empty())
    {
        gl::glBindVertexArray(m_impl->glVertexArray);

        auto & attributes = m_impl->program->interface.attributes();
        for (const auto & a : m_impl->dirtyValueAttributes)
        {
            Assert(a < attributes.size(), "");

            const auto & valueBinding =
                boost::get<detail::VertexAttributeValueBinding>(m_impl->attributeBindings[a]);

            GLBindVertexAttribute(
                m_impl->glVertexArray,
                attributes[a],
                m_impl->valueAttributes.ptr(valueBinding.offset)
            );
        }

        m_impl->dirtyValueAttributes.clear();
    }

    m_impl->drawContext.scheduleDraw(*this);
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

    return stream.str();
}

Draw::Draw(const std::shared_ptr<detail::DrawImpl> & impl):
    m_impl(impl)
{

}

void Draw::build() const
{
    /*
        ToDo
            Move all gl-stuff to DrawContext/GLStateManager
    */

    gl::glGenVertexArrays(1, &m_impl->glVertexArray);
    Assert(m_impl->glVertexArray != 0, "");

    for (size_t a = 0; a < m_impl->attributeBindings.size(); a++)
    {
        const auto & attribute = m_impl->program->interface.attributes()[a];
        const auto & attributeBinding = m_impl->attributeBindings[a];

        Assert (attributeBinding.which() != 0, "Attribute '" + attribute.name() + "' is not supplied");

        if (attributeBinding.which() == 1) continue; // value attribute

        const auto & bufferBinding = boost::get<detail::VertexAttributeBufferBinding>(attributeBinding);

        const auto baseOffset = bufferBinding.ranged ? bufferBinding.first * bufferBinding.buffer->layout.stride() : 0;

        GLBindVertexAttribute(m_impl->glVertexArray,
                              attribute,
                              *bufferBinding.buffer,
                              bufferBinding.fieldIndex,
                              bufferBinding.divisor,
                              baseOffset);
    }
}

void Draw::addVertexBuffer(const Buffer & buffer, bool ranged, u32 first, u32 count, u32 divisor)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    const auto & layout = buffer.layout();
    const auto & interface = m_impl->program->interface;

    for (size_t f = 0; f < layout.fields().size(); f++)
    {
        const auto & field = layout.fields()[f];

        const auto * attribute = interface.attribute(field.name());
        if (!attribute) continue;

        auto & binding = m_impl->attributeBindings[attribute->index()];
        Assert(binding.which() == 0, "Vertex attribute '" + field.name() + "' is already supplied.");

        binding = detail::VertexAttributeBufferBinding{};

        auto & bufferBinding = boost::get<detail::VertexAttributeBufferBinding>(binding);

        bufferBinding.buffer = buffer.m_impl;
        bufferBinding.fieldIndex = (u32)f;
        bufferBinding.ranged = ranged;
        bufferBinding.first = first;
        bufferBinding.count = count;
        bufferBinding.divisor = divisor;
    }

}

void Draw::setAttribute(const ProgramInterfaceVertexAttribute & attribute, const void * data)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & binding = m_impl->attributeBindings[attribute.index()];

    if (binding.which() == 0) // Attribute not yet assigned, allocate new space in value blob
    {
        const auto offset = m_impl->valueAttributes.size();
        m_impl->valueAttributes.resize(offset + attribute.type().size());

        auto valueBinding = detail::VertexAttributeValueBinding();
		valueBinding.offset = offset;
		valueBinding.attributeIndex = attribute.index();

		binding = valueBinding;
    }

    Assert(binding.which() != 2, "Vertex attribute '" + attribute.name() + "' is already bound to buffer");

    auto & valueBinding = boost::get<detail::VertexAttributeValueBinding>(binding);

    m_impl->valueAttributes.write(valueBinding.offset, data, attribute.type().size());
    m_impl->dirtyValueAttributes.emplace_back(attribute.index());
}

}

