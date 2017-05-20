#include <Deliberation/Draw/Draw.h>

#include <algorithm>
#include <iostream>
#include <sstream>

#include <glbinding/Meta.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Detail/BufferImpl.h>
#include <Deliberation/Draw/Detail/DrawImpl.h>
#include <Deliberation/Draw/Detail/FramebufferImpl.h>
#include <Deliberation/Draw/Detail/ProgramImpl.h>
#include <Deliberation/Draw/Detail/TextureImpl.h>
#include <Deliberation/Draw/Detail/UniformBufferBinding.h>
#include <Deliberation/Draw/Detail/VertexAttributeBinding.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/GL/GLBindVertexAttribute.h>
#include <Deliberation/Draw/GL/GLType.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Scene/Util/RangedGpuScope.h>

namespace deliberation
{
Draw::Draw() : m_impl(nullptr) {}

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
    return m_impl->framebufferBinding.framebuffer();
}

const Framebuffer & Draw::framebuffer() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return m_impl->framebufferBinding.framebuffer();
}

Uniform Draw::uniform(const std::string & name)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    const auto & uniforms = m_impl->program->interface.uniforms();

    for (size_t u = 0; u < uniforms.size(); u++)
    {
        if (uniforms[u].name() == name)
        {
            return Uniform(m_impl, u);
        }
    }

    Fail("No such uniform");
}

Sampler Draw::sampler(const std::string & name)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto location = m_impl->program->interface.samplerRef(name).location();

    for (auto & sampler : m_impl->samplers)
    {
        if (sampler->location == location) return Sampler(sampler);
    }

    Fail("sampler");
}

Buffer Draw::setIndices(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & drawContext = m_impl->drawContext;
    auto   buffer = drawContext.createBuffer(data.layout());
    buffer.upload(data);
    setIndexBuffer(buffer);

    return buffer;
}

Buffer Draw::addVertices(const LayoutedBlob & data)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & drawContext = m_impl->drawContext;
    auto   buffer = drawContext.createBuffer(data.layout());
    buffer.upload(data);
    addVertexBuffer(buffer);

    return buffer;
}

Buffer Draw::addInstances(const LayoutedBlob & data, u32 divisor)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto & drawContext = m_impl->drawContext;
    auto   buffer = drawContext.createBuffer(data.layout());
    buffer.upload(data);
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

void Draw::addInstanceBufferRange(
    const Buffer & buffer, u32 first, u32 count, u32 divisor)
{
    addVertexBuffer(buffer, true, first, count, divisor);
}

VertexAttribute Draw::attribute(const std::string & name)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    const auto & attribute = program().interface().attributeRef(name);

    return VertexAttribute(m_impl, attribute);
}

void Draw::setFramebuffer(
    Framebuffer & framebuffer,
    const FramebufferMappings & mapping)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    setFramebufferBinding(framebufferBinding(framebuffer, mapping));
}

FramebufferBinding Draw::framebufferBinding(Framebuffer & framebuffer, const FramebufferMappings & bindingParams)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return FramebufferBinding(m_impl->program->interface.fragmentOutputs(), framebuffer, bindingParams);
}

void Draw::setFramebufferBinding(const FramebufferBinding & binding)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    m_impl->framebufferBinding = binding;
}

void Draw::setUniformBuffer(
    const std::string & name, const Buffer & buffer, unsigned int begin)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    auto index = m_impl->program->interface.uniformBlockRef(name).index();

    UniformBufferBinding binding{buffer.m_impl, begin};

    m_impl->uniformBuffers[index].reset(binding);
}

UniformBufferHandle Draw::uniformBuffer(const std::string & name)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    return UniformBufferHandle(m_impl, m_impl->program->interface.uniformBlockRef(name).index());
}

void Draw::render() const
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    DELIBERATION_GPU_SCOPE(m_impl->name.empty() ? "<Unnamed Draw>" : m_impl->name);

    auto & glStateManager = m_impl->drawContext.m_glStateManager;

    if (m_impl->glVertexArray == 0) build();

    if (m_impl->indexBufferBindingDirty && m_impl->indexBufferBinding.buffer)
    {
        glStateManager.bindVertexArray(m_impl->glVertexArray);
        glStateManager.bindBuffer(
            gl::GL_ELEMENT_ARRAY_BUFFER,
            m_impl->indexBufferBinding.buffer->glName);
        m_impl->indexBufferBindingDirty = false;
    }

    if (!m_impl->dirtyValueAttributes.empty())
    {
        glStateManager.bindVertexArray(m_impl->glVertexArray);

        auto & attributes = m_impl->program->interface.attributes();
        for (const auto & a : m_impl->dirtyValueAttributes)
        {
            Assert(a < attributes.size(), "");

            const auto & valueBinding = boost::get<VertexAttributeValueBinding>(
                m_impl->attributeBindings[a]);

            GLBindVertexAttribute(
                m_impl->glVertexArray,
                attributes[a],
                m_impl->valueAttributes.ptr(valueBinding.offset));
        }

        m_impl->dirtyValueAttributes.clear();
    }

    glStateManager.enableTextureCubeMapSeamless(true);

    // Apply Depth State
    {
        auto & state = m_impl->state.depthState();

        if (state.depthWrite() == DepthWrite::Disabled &&
            state.depthTest() == DepthTest::Disabled)
        {
            glStateManager.enableDepthTest(false);
        }
        else
        {
            glStateManager.enableDepthTest(true);
            glStateManager.setDepthFunc((gl::GLenum)state.depthTest());
            glStateManager.setDepthMask(
                state.depthWrite() == DepthWrite::Enabled);
        }
    }

    // Apply Blend State
    {
        auto & state = m_impl->state.blendState();

        glStateManager.enableBlend(state.enabled());

        if (state.enabled())
        {
            glStateManager.setBlendEquation((gl::GLenum)state.equation());
            glStateManager.setBlendFunc(
                (gl::GLenum)state.sfactor(), (gl::GLenum)state.dfactor());
        }
    }

    // Apply cull state
    {
        auto & state = m_impl->state.cullState();

        glStateManager.enableCullFace(state.enabled());

        if (state.enabled())
        {
            glStateManager.setCullFace((gl::GLenum)state.cullFace());
        }
    }

    // Apply rasterizer state
    {
        auto & state = m_impl->state.rasterizerState();

        glStateManager.setPointSize(state.pointSize());
        glStateManager.setLineWidth(state.lineWidth());

        glStateManager.enableScissorTest(state.scissorRectEnabled());
        if (state.scissorRectEnabled())
        {
            const auto & scissorRect = state.scissorRect();
            glStateManager.setScissor(
                scissorRect.x, scissorRect.y, scissorRect.z, scissorRect.w);
        }
    }

    // Apply stencil state
    {
        auto & state = m_impl->state.stencilState();

        glStateManager.enableStencilTest(state.enabled());

        if (state.enabled())
        {
            auto & front = state.frontFace();
            auto & back = state.backFace();

            if (state.differentFaceFuncs())
            {
                glStateManager.setStencilFuncSeparate(
                    gl::GL_FRONT,
                    (gl::GLenum)front.func,
                    front.ref,
                    front.readMask);
                glStateManager.setStencilFuncSeparate(
                    gl::GL_BACK,
                    (gl::GLenum)back.func,
                    back.ref,
                    back.readMask);
            }
            else
            {
                glStateManager.setStencilFunc(
                    (gl::GLenum)state.func(), state.ref(), state.readMask());
            }

            if (state.differentFaceMasks())
            {
                glStateManager.setStencilMaskSeparate(
                    gl::GL_FRONT, front.writeMask);
                glStateManager.setStencilMaskSeparate(
                    gl::GL_BACK, back.writeMask);
            }
            else
            {
                glStateManager.setStencilMask(state.writeMask());
            }

            if (state.differentFaceOps())
            {
                glStateManager.setStencilOpSeparate(
                    gl::GL_FRONT,
                    (gl::GLenum)front.sfail,
                    (gl::GLenum)front.dpfail,
                    (gl::GLenum)front.dppass);
                glStateManager.setStencilOpSeparate(
                    gl::GL_BACK,
                    (gl::GLenum)back.sfail,
                    (gl::GLenum)back.dpfail,
                    (gl::GLenum)back.dppass);
            }
            else
            {
                glStateManager.setStencilOp(
                    (gl::GLenum)state.sfail(),
                    (gl::GLenum)state.dpfail(),
                    (gl::GLenum)state.dppass());
            }
        }
    }

    // Apply viewport
    {
        auto & state = m_impl->state;
        auto & framebuffer = m_impl->framebufferBinding.framebuffer();
        // gl::glProvokingVertex(m_provokingVertex);

        if (state.hasViewport())
        {
            glStateManager.setViewport(
                state.viewport().x(),
                state.viewport().y(),
                state.viewport().width(),
                state.viewport().height());
        }
        else
        {
            if (framebuffer.isBackbuffer())
            {
                glStateManager.setViewport(
                    0,
                    0,
                    m_impl->drawContext.backbuffer().width(),
                    m_impl->drawContext.backbuffer().height());
            }
            else
            {
                glStateManager.setViewport(
                    0, 0, framebuffer.width(), framebuffer.height());
            }
        }
    }

    glStateManager.useProgram(m_impl->program->glProgramName);

    // Setup texture units
    for (auto b = 0u; b < m_impl->samplers.size(); b++)
    {
        auto & sampler = m_impl->samplers[b];
        auto   texture = sampler->textureImpl;

        Assert((bool)texture, "Sampler has no Texture bound to it");

        glStateManager.setActiveTexture(b);
        glStateManager.bindTexture((gl::GLenum)texture->type, texture->glName);
        glStateManager.bindSampler(b, sampler->glSampler.name());
        gl::glUniform1i(sampler->location, b);
    }

    // Setup RenderTarget / Framebuffer
    m_impl->framebufferBinding.framebuffer().m_impl->bind(m_impl->framebufferBinding.drawBuffersGL());

    // Set uniforms
    {
        for (size_t u = 0; u < m_impl->uniforms.size(); u++)
        {
            const auto & uniformBinding = m_impl->uniforms[u];
            const auto & uniformInterface =
                m_impl->program->interface.uniforms()[u];

            Assert(
                uniformBinding.assigned,
                "Uniform " + uniformInterface.name() + " not set");
            Assert(uniformBinding.count > 0, "");

            auto   count = uniformBinding.count;
            auto * data = m_impl->uniformData.rawData().ptr(
                m_impl->uniformLayout.field(u).offset());
            auto location = m_impl->program->interface.uniforms()[u].location();

            switch (TypeToGLType(uniformInterface.type()))
            {
            case gl::GL_BOOL:
            {
                gl::GLint boolInt = *(const gl::GLboolean *)data ? 1 : 0;
                gl::glUniform1iv(location, count, &boolInt);
                break;
            }
            case gl::GL_INT:
                gl::glUniform1iv(location, count, ((const gl::GLint *)data));
                break;
            case gl::GL_UNSIGNED_INT:
                gl::glUniform1uiv(location, count, ((const gl::GLuint *)data));
                break;
            case gl::GL_INT_VEC2:
            {
                auto idata = (const gl::GLint *)data;
                gl::glUniform2iv(location, count, idata);
                break;
            }
            case gl::GL_FLOAT:
                gl::glUniform1fv(location, count, ((const gl::GLfloat *)data));
                break;
            case gl::GL_FLOAT_VEC2:
            {
                auto fdata = (const gl::GLfloat *)data;
                gl::glUniform2fv(location, count, fdata);
                break;
            }
            case gl::GL_FLOAT_VEC3:
            {
                auto fdata = (const gl::GLfloat *)data;
                gl::glUniform3fv(location, count, fdata);
                break;
            }
            case gl::GL_FLOAT_VEC4:
            {
                auto fdata = (const gl::GLfloat *)data;
                gl::glUniform4fv(location, count, fdata);
                break;
            }
            case gl::GL_FLOAT_MAT2:
                gl::glUniformMatrix2fv(
                    location, count, gl::GL_FALSE, (const gl::GLfloat *)data);
                break;
            case gl::GL_FLOAT_MAT3:
                gl::glUniformMatrix3fv(
                    location, count, gl::GL_FALSE, (const gl::GLfloat *)data);
                break;
            case gl::GL_FLOAT_MAT4:
                gl::glUniformMatrix4fv(
                    location, count, gl::GL_FALSE, (const gl::GLfloat *)data);
                break;
            default:
                Fail(
                    std::string("Not implemented for type ") +
                    uniformInterface.type().name());
            }
        }
    }

    // Set uniform buffers
    {
        for (auto b = 0; b < m_impl->uniformBuffers.size(); b++)
        {
            auto & binding = m_impl->uniformBuffers[b];

            Assert(
                binding.engaged(),
                "UniformBuffer " +
                    m_impl->program->interface.uniformBlocks()[b].name() +
                    " not bound");

            auto & buffer = *binding.get().buffer;
            auto   size = buffer.count * buffer.layout.stride();

            Assert(size > binding.get().begin, "begin beyond buffer bounds");

            gl::glUniformBlockBinding(m_impl->program->glProgramName, b, b);
            gl::glBindBufferRange(
                gl::GL_UNIFORM_BUFFER,
                b,
                buffer.glName,
                binding.get().begin,
                buffer.layout.stride());
        }
    }

    // Dispatch draw
    u32 instanceCount = 0;
    u32 vertexCount = 0;

    const auto useIndices = m_impl->indexBufferBinding.buffer != nullptr;

    for (const auto & attributeBinding : m_impl->attributeBindings)
    {
        if (attributeBinding.which() == 1) continue; // Value binding

        const auto & bufferBinding =
            boost::get<VertexAttributeBufferBinding>(attributeBinding);

        if (bufferBinding.ranged)
        {
            Assert(
                bufferBinding.count + bufferBinding.first <
                    bufferBinding.buffer->count,
                "Buffer too small");

            if (bufferBinding.divisor > 0)
            {
                Assert(
                    instanceCount == 0 || bufferBinding.count == instanceCount,
                    "Instance count mismatch");
                instanceCount = bufferBinding.count;
            }
            else if (!m_impl->indexBufferBinding.buffer)
            {
                Assert(
                    vertexCount == 0 || bufferBinding.count == vertexCount,
                    "Vertex count mismatch");
                vertexCount = bufferBinding.count;
            }
        }
        else
        {
            if (bufferBinding.divisor > 0)
            {
                Assert(
                    instanceCount == 0 ||
                        bufferBinding.buffer->count == instanceCount,
                    "Instance count mismatch");
                instanceCount = bufferBinding.buffer->count;
            }
            else
            {
                if (!useIndices)
                {
                    Assert(
                        vertexCount == 0 ||
                            bufferBinding.buffer->count == vertexCount,
                        "Vertex count mismatch");
                    vertexCount = bufferBinding.buffer->count;
                }
            }
        }
    }

    glStateManager.bindVertexArray(m_impl->glVertexArray);

    if (useIndices)
    {
        const auto type =
            m_impl->indexBufferBinding.buffer->layout.fields()[0].type();

        u32 elementCount = 0;
        u32 offset = 0;

        if (m_impl->indexBufferBinding.ranged)
        {
            elementCount = m_impl->indexBufferBinding.count;
            offset = (u32)(m_impl->indexBufferBinding.first * type.size());
        }
        else
        {
            elementCount = m_impl->indexBufferBinding.buffer->count;
            offset = 0;
        }

        if (instanceCount != 0)
        {
            gl::glDrawElementsInstanced(
                (gl::GLenum)m_impl->state.rasterizerState().primitive(),
                elementCount,
                TypeToGLType(type),
                (void *)(intptr_t)offset,
                instanceCount);
        }
        else
        {
            gl::glDrawElements(
                (gl::GLenum)m_impl->state.rasterizerState().primitive(),
                elementCount,
                TypeToGLType(type),
                (void *)(intptr_t)offset);
        }
    }
    else
    {
        if (instanceCount != 0)
        {
            gl::glDrawArraysInstanced(
                (gl::GLenum)m_impl->state.rasterizerState().primitive(),
                0u,
                vertexCount,
                instanceCount);
        }
        else
        {
            gl::glDrawArrays(
                (gl::GLenum)m_impl->state.rasterizerState().primitive(),
                0u,
                vertexCount);
        }
    }
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

Draw::Draw(const std::shared_ptr<DrawImpl> & impl) : m_impl(impl) {}

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

        Assert(
            attributeBinding.which() != 0,
            "Attribute '" + attribute.name() + "' is not supplied");

        if (attributeBinding.which() == 1) continue; // value attribute

        const auto & bufferBinding =
            boost::get<VertexAttributeBufferBinding>(attributeBinding);

        const auto baseOffset =
            bufferBinding.ranged
                ? bufferBinding.first * bufferBinding.buffer->layout.stride()
                : 0;

        GLBindVertexAttribute(
            m_impl->drawContext.m_glStateManager,
            m_impl->glVertexArray,
            attribute,
            *bufferBinding.buffer,
            bufferBinding.fieldIndex,
            bufferBinding.divisor,
            baseOffset);
    }
}

void Draw::addVertexBuffer(
    const Buffer & buffer, bool ranged, u32 first, u32 count, u32 divisor)
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
        Assert(
            binding.which() == 0,
            "Vertex attribute '" + field.name() + "' is already supplied.");

        binding = VertexAttributeBufferBinding{};

        auto & bufferBinding =
            boost::get<VertexAttributeBufferBinding>(binding);

        bufferBinding.buffer = buffer.m_impl;
        bufferBinding.fieldIndex = (u32)f;
        bufferBinding.ranged = ranged;
        bufferBinding.first = first;
        bufferBinding.count = count;
        bufferBinding.divisor = divisor;
    }
}

void Draw::setAttribute(
    const ProgramInterfaceVertexAttribute & attribute, const void * data)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");
    m_impl->setAttribute(attribute, data);
}
}
