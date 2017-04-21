#include "DrawExecution.h"

#include <iostream>

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>
#include <glbinding/Meta.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/GL/GLType.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

#include "BufferImpl.h"
#include "DrawImpl.h"
#include "FramebufferImpl.h"
#include "ProgramImpl.h"
#include "TextureImpl.h"

namespace deliberation
{

namespace detail
{

DrawExecution::DrawExecution(GLStateManager & glStateManager,
                             const Draw & draw):
    m_glStateManager(glStateManager),
    m_drawImpl(*draw.m_impl)
{
}

void DrawExecution::perform()
{
    m_glStateManager.enableTextureCubeMapSeamless(true);

    // Apply State
    applyDepthState();
    applyBlendState();
    applyCullState();
    applyRasterizerState();
    applyStencilState();
    applyViewport();

    m_glStateManager.useProgram(m_drawImpl.program->glProgramName);

    // Setup texture units
    for (auto b = 0u; b < m_drawImpl.samplers.size(); b++)
    {
        auto & sampler = m_drawImpl.samplers[b];
        auto * texture = sampler.texture;

        Assert(texture, "");

        if (texture->glName == 0)
        {
            texture->allocate();
        }

        m_glStateManager.setActiveTexture(b);
        m_glStateManager.bindTexture(texture->type, texture->glName);
        gl::glUniform1i(sampler.location, b);
    }

    // Setup RenderTarget / Framebuffer
    Assert(m_drawImpl.framebuffer.m_impl.get(), "");
    m_drawImpl.framebuffer.m_impl->bind(m_glStateManager);

    // Set uniforms
    {
        /*
            TODO
                Port to GLStateManager
        */

        for (auto & uniform : m_drawImpl.uniforms)
        {
            Assert(uniform.isAssigned, "Uniform " + m_drawImpl.program->interface.uniformByLocation(uniform.location)->name() + " not set");
            Assert(uniform.count > 0, "");

            auto count = uniform.count;
            auto * data = uniform.blob.ptr();
            auto location = uniform.location;

            switch (TypeToGLType(uniform.type))
            {
            case gl::GL_INT:
                gl::glUniform1iv(location, count, ((const gl::GLint*)data));
                break;
            case gl::GL_UNSIGNED_INT:
                gl::glUniform1uiv(location, count, ((const gl::GLuint*)data));
                break;
            case gl::GL_INT_VEC2:
            {
                auto idata = (const gl::GLint*)data;
                gl::glUniform2iv(location, count, idata);
                break;
            }
            case gl::GL_FLOAT:
                gl::glUniform1fv(location, count, ((const gl::GLfloat*)data));
                break;
            case gl::GL_FLOAT_VEC2:
            {
                auto fdata = (const gl::GLfloat*)data;
                gl::glUniform2fv(location, count, fdata);
                break;
            }
            case gl::GL_FLOAT_VEC3:
            {
                auto fdata = (const gl::GLfloat*)data;
                gl::glUniform3fv(location, count, fdata);
                break;
            }
            case gl::GL_FLOAT_VEC4:
            {
                auto fdata = (const gl::GLfloat*)data;
                gl::glUniform4fv(location, count, fdata);
                break;
            }
            case gl::GL_FLOAT_MAT2:
                gl::glUniformMatrix2fv(location, count, gl::GL_FALSE, (const gl::GLfloat*)data);
                break;
            case gl::GL_FLOAT_MAT3:
                gl::glUniformMatrix3fv(location, count, gl::GL_FALSE, (const gl::GLfloat*)data);
                break;
            case gl::GL_FLOAT_MAT4:
                gl::glUniformMatrix4fv(location, count, gl::GL_FALSE, (const gl::GLfloat*)data);
                break;
            default:
                Fail(std::string("Not implemented for type ") + uniform.type.name());
            }
        }
    }

    // Set uniform buffers
    {
        for (auto b = 0; b < m_drawImpl.uniformBuffers.size(); b++)
        {
            auto & binding = m_drawImpl.uniformBuffers[b];

            Assert(binding.engaged(), "UniformBuffer " + m_drawImpl.program->interface.uniformBlocks()[b].name() + " not bound");

            auto & buffer = *binding.get().buffer;
            auto size = buffer.count * buffer.layout.stride();

            Assert(size > binding.get().begin, "begin beyond buffer bounds");

            gl::glUniformBlockBinding(m_drawImpl.program->glProgramName, b, b);
            gl::glBindBufferRange(gl::GL_UNIFORM_BUFFER, b, buffer.glName, binding.get().begin, buffer.layout.stride());
        }
    }

    // Dispatch draw
    u32 instanceCount = 0;
    u32 vertexCount = 0;

    const auto useIndices = m_drawImpl.indexBufferBinding.buffer != nullptr;

    for (const auto & attributeBinding : m_drawImpl.attributeBindings)
    {
        if (attributeBinding.which() == 1) continue; // Value binding

        const auto & bufferBinding = boost::get<detail::VertexAttributeBufferBinding>(attributeBinding);

        if (bufferBinding.ranged)
        {
            Assert(bufferBinding.count + bufferBinding.first < bufferBinding.buffer->count, "Buffer too small");

            if (bufferBinding.divisor > 0)
            {
                Assert(instanceCount == 0 || bufferBinding.count == instanceCount, "Instance count mismatch");
                instanceCount = bufferBinding.count;
            }
            else if (!m_drawImpl.indexBufferBinding.buffer)
            {
                Assert(vertexCount == 0 || bufferBinding.count == vertexCount, "Vertex count mismatch");
                vertexCount = bufferBinding.count;
            }
        }
        else
        {
            if (bufferBinding.divisor > 0)
            {
                Assert(instanceCount == 0 || bufferBinding.buffer->count == instanceCount, "Instance count mismatch");
                instanceCount = bufferBinding.buffer->count;
            }
            else
            {
                if (!useIndices)
                {
                    Assert(vertexCount == 0 || bufferBinding.buffer->count == vertexCount, "Vertex count mismatch");
                    vertexCount = bufferBinding.buffer->count;
                }
            }
        }
    }

    gl::glBindVertexArray(m_drawImpl.glVertexArray);

    if (useIndices)
    {
        const auto type = m_drawImpl.indexBufferBinding.buffer->layout.fields()[0].type();

        u32 elementCount = 0;
        u32 offset = 0;

        if (m_drawImpl.indexBufferBinding.ranged)
        {
            elementCount = m_drawImpl.indexBufferBinding.count;
            offset = (u32)(m_drawImpl.indexBufferBinding.first * type.size());
        }
        else
        {
            elementCount = m_drawImpl.indexBufferBinding.buffer->count;
            offset = 0;
        }

        if (instanceCount != 0)
        {
            gl::glDrawElementsInstanced(m_drawImpl.state.rasterizerState().primitive(),
                                        elementCount,
                                        TypeToGLType(type),
                                        (void*)(intptr_t)offset,
                                        instanceCount);

        }
        else
        {
            gl::glDrawElements(m_drawImpl.state.rasterizerState().primitive(),
                               elementCount,
                               TypeToGLType(type),
                               (void*)(intptr_t)offset);
        }
    }
    else
    {
        if (instanceCount != 0)
        {
            gl::glDrawArraysInstanced(m_drawImpl.state.rasterizerState().primitive(),
                                      0u,
                                      vertexCount,
                                      instanceCount);
        }
        else
        {
            gl::glDrawArrays(m_drawImpl.state.rasterizerState().primitive(),
                             0u,
                             vertexCount);
        }
    }
}

void DrawExecution::applyDepthState()
{
    auto & state = m_drawImpl.state.depthState();

    if (state.depthWrite() == DepthWrite::Disabled && state.depthTest() == DepthTest::Disabled)
    {
        m_glStateManager.enableDepthTest(false);
    }
    else
    {
        m_glStateManager.enableDepthTest(true);

        auto depthFunc = gl::GL_NONE;

        switch(state.depthTest())
        {
            case DepthTest::Less:           depthFunc = gl::GL_LESS; break;
            case DepthTest::Always:         depthFunc = gl::GL_ALWAYS; break;
            case DepthTest::Equal:          depthFunc = gl::GL_EQUAL; break;
            case DepthTest::LessOrEqual:    depthFunc = gl::GL_LEQUAL; break;
            case DepthTest::Greater:        depthFunc = gl::GL_GREATER; break;
            case DepthTest::NotEqual:       depthFunc = gl::GL_NOTEQUAL; break;
            case DepthTest::GreaterOrEqual: depthFunc = gl::GL_GEQUAL; break;
        }
        m_glStateManager.setDepthFunc(depthFunc);
        m_glStateManager.setDepthMask(state.depthWrite() == DepthWrite::Enabled);
    }
}

void DrawExecution::applyBlendState()
{
    auto & state = m_drawImpl.state.blendState();

    m_glStateManager.enableBlend(state.enabled());

    if (!state.enabled())
    {
        return;
    }

    m_glStateManager.setBlendEquation(state.equation());
    m_glStateManager.setBlendFunc(state.sfactor(), state.dfactor());
}

void DrawExecution::applyCullState()
{
    auto & state = m_drawImpl.state.cullState();

    m_glStateManager.enableCullFace(state.enabled());

    if (!state.enabled())
    {
        return;
    }

    m_glStateManager.setCullFace(state.cullFace());
}

void DrawExecution::applyRasterizerState()
{
    auto & state = m_drawImpl.state.rasterizerState();

    m_glStateManager.setPointSize(state.pointSize());
    m_glStateManager.setLineWidth(state.lineWidth());

    m_glStateManager.enableScissorTest(state.scissorRectEnabled());
    if (state.scissorRectEnabled())
    {
        const auto & scissorRect = state.scissorRect();
        m_glStateManager.setScissor(scissorRect.x, scissorRect.y, scissorRect.z, scissorRect.w);
    }
}

void DrawExecution::applyStencilState()
{
    auto & state = m_drawImpl.state.stencilState();

    m_glStateManager.enableStencilTest(state.enabled());

    if (!state.enabled())
    {
        return;
    }

    auto & front = state.frontFace();
    auto & back = state.backFace();

    if (state.differentFaceFuncs())
    {
        m_glStateManager.setStencilFuncSeparate(gl::GL_FRONT, front.func, front.ref, front.readMask);
        m_glStateManager.setStencilFuncSeparate(gl::GL_BACK, back.func, back.ref, back.readMask);
    }
    else
    {
        m_glStateManager.setStencilFunc(state.func(), state.ref(), state.readMask());
    }

    if (state.differentFaceMasks())
    {
        m_glStateManager.setStencilMaskSeparate(gl::GL_FRONT, front.writeMask);
        m_glStateManager.setStencilMaskSeparate(gl::GL_BACK, back.writeMask);
    }
    else
    {
        m_glStateManager.setStencilMask(state.writeMask());
    }

    if (state.differentFaceOps())
    {
        m_glStateManager.setStencilOpSeparate(gl::GL_FRONT, front.sfail, front.dpfail, front.dppass);
        m_glStateManager.setStencilOpSeparate(gl::GL_BACK, back.sfail, back.dpfail, back.dppass);
    }
    else
    {
        m_glStateManager.setStencilOp(state.sfail(), state.dpfail(), state.dppass());
    }
}

void DrawExecution::applyViewport()
{
    auto & state = m_drawImpl.state;
    auto & framebuffer = m_drawImpl.framebuffer;
    //gl::glProvokingVertex(m_provokingVertex);

    if (state.hasViewport())
    {
        m_glStateManager.setViewport(state.viewport().x(), state.viewport().y(),
                                     state.viewport().width(), state.viewport().height());
    }
    else
    {
        if (framebuffer.isBackbuffer())
        {
            m_glStateManager.setViewport(0, 0, m_drawImpl.context.backbuffer().width(),
                                               m_drawImpl.context.backbuffer().height());
        }
        else
        {
            m_glStateManager.setViewport(0, 0, framebuffer.width(),
                                               framebuffer.height());
        }
    }
}

}

}

