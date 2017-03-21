#include "DrawExecution.h"

#include <iostream>

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>
#include <glbinding/Meta.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/GL/GLType.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Context.h>
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
    if (m_drawImpl.indexBufferBinding.buffer)
    {
        if (!m_drawImpl.instanceBuffers.empty())
        {
            drawElementsInstanced();
        }
        else
        {
            drawElements();
        }
    }
    else
    {
        if (!m_drawImpl.instanceBuffers.empty())
        {
            drawArraysInstanced();
        }
        else
        {
            drawArrays();
        }
    }
}

/*
    TODO
        Use GLStateManager for VAO
*/

void DrawExecution::drawElementsInstanced() const
{
    gl::glBindVertexArray(m_drawImpl.glVertexArray);

    const auto offset = m_drawImpl.indexBufferBinding.ranged ? m_drawImpl.indexBufferBinding.first : 0;

    const auto type = elementType();

    gl::glDrawElementsInstanced(m_drawImpl.state.rasterizerState().primitive(),
                                          elementCount(),
                                          TypeToGLType(type),
                                          (void*)(intptr_t)(offset * type.size()),
                                          instanceCount());
}

void DrawExecution::drawElements() const
{
    gl::glBindVertexArray(m_drawImpl.glVertexArray);

    const auto offset = m_drawImpl.indexBufferBinding.ranged ? m_drawImpl.indexBufferBinding.first : 0;
    const auto type = elementType();

    gl::glDrawElements(m_drawImpl.state.rasterizerState().primitive(),
                       elementCount(),
                       TypeToGLType(type),
                       (void*)(intptr_t)(offset * type.size()));
}

void DrawExecution::drawArrays() const
{
    gl::glBindVertexArray(m_drawImpl.glVertexArray);
    gl::glDrawArrays(m_drawImpl.state.rasterizerState().primitive(),
                     0u,
                     vertexCount());
}

void DrawExecution::drawArraysInstanced() const
{
    gl::glBindVertexArray(m_drawImpl.glVertexArray);
    gl::glDrawArraysInstanced(m_drawImpl.state.rasterizerState().primitive(),
                                        0u,
                                        vertexCount(),
                                        instanceCount());
}

unsigned int DrawExecution::elementCount() const
{
    const auto & binding = m_drawImpl.indexBufferBinding;

    Assert(binding.buffer.get(), "No index buffer set");
    Assert(binding.buffer->count > 0, "Index buffer is empty");

    if (binding.ranged) return binding.count;
    else return binding.buffer->count;
}

unsigned int DrawExecution::vertexCount() const
{
    Assert(!m_drawImpl.vertexBuffers.empty(), "No vertex buffer bound to command");

    auto ref = m_drawImpl.vertexBuffers[0].ranged ?
               m_drawImpl.vertexBuffers[0].count :
               m_drawImpl.vertexBuffers[0].buffer->count;

    Assert(ref > 0, "Vertex buffer is empty");

    {
        for (auto b = 1u; b < m_drawImpl.vertexBuffers.size(); b++)
        {
            auto cmp = m_drawImpl.vertexBuffers[b].ranged ?
                       m_drawImpl.vertexBuffers[b].count :
                       m_drawImpl.vertexBuffers[0].buffer->count;
            Assert(cmp == ref, "");
        }
    }

    return ref;
}

unsigned int DrawExecution::instanceCount() const
{
    assert(!m_drawImpl.instanceBuffers.empty());

    auto & buffer = *m_drawImpl.instanceBuffers[0].buffer;
    auto divisor = m_drawImpl.instanceBuffers[0].divisor;

    Assert(buffer.count > 0, "Instance buffer is empty");
    Assert(divisor > 0, "Divisor of instance buffer is zero");

    auto ref = buffer.count * divisor;

    for (auto b = 1u; b < m_drawImpl.instanceBuffers.size(); b++)
    {
        auto & buffer = *m_drawImpl.instanceBuffers[b].buffer;
        auto divisor = m_drawImpl.instanceBuffers[b].divisor;

        auto test = buffer.count * divisor;

        Assert(test == ref, "Differing instance count in buffer " + std::to_string(b) + ": " + std::to_string(test) + " != " + std::to_string(ref));
    }

    return ref;
}

Type DrawExecution::elementType() const
{
    Assert(m_drawImpl.indexBufferBinding.buffer.get(), "No index buffer set");
    Assert(m_drawImpl.indexBufferBinding.buffer->layout.fields().size() == 1u, "Invalid index buffer layout");

    return m_drawImpl.indexBufferBinding.buffer->layout.fields()[0].type();
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

