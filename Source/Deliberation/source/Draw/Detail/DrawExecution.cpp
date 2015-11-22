#include "DrawExecution.h"

#include <glbinding/gl/functions.h>

#include <globjects/Sampler.h>

#include "GL/GLStateManager.h"

#include "DrawCommand.h"

DrawExecution::DrawExecution(GLStateManager & glStateManager,
                             const DrawCommand & command):
    m_glStateManager(glStateManager),
    m_command(command)
{

}

void DrawExecution::perform()
{
    m_glStateManager.enableTextureCubeMapSeamless(true);

    applyDepthState();
    applyBlendState();
    applyCullState();
    applyRasterizerState();
    applyStencilState();

    m_command.m_state.apply();
    m_command.m_program.use();

    // Setup texture units
    for (auto u = 0u; u < m_command.m_textureUnitBindings.size(); u++)
    {
        auto & binding = m_command.m_textureUnitBindings[u];
        auto * texture = binding.texture;
        auto * sampler = binding.sampler;

        Assert(texture, "No texture set for unit " + std::to_string(u));

        texture->bindActive(gl::GL_TEXTURE0 + u);

        if (sampler)
        {
            sampler->bind(u);
        }
        else
        {
            globjects::Sampler::unbind(u);
        }
    }

    // Setup MRT
    m_command.m_output.get().apply();

    // Set uniforms
    for (auto u = 0u; u < m_command.m_uniforms.size(); u++)
    {
        Assert(m_command.m_uniforms[u].isValid(), "Uniform " + std::to_string(u) + " not set");
        m_command.m_program.setUniform(u, m_command.m_uniforms[u].type(), m_command.m_uniforms[u].data());
    }

    // Dispatch draw
    if (m_command.m_indexBuffer)
    {
        if (!m_command.m_instanceBuffers.empty())
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
        if (!m_command.m_instanceBuffers.empty())
        {
            drawArraysInstanced();
        }
        else
        {
            drawArrays();
        }
    }
}

void DrawExecution::drawElementsInstanced() const
{
    m_command.m_vao.drawElementsInstanced(m_command.m_state.primitive(),
                                          elementCount(),
                                          gl::GL_UNSIGNED_INT,
                                          nullptr,
                                          instanceCount());
}

void DrawExecution::drawElements() const
{
    m_command.m_vao.drawElements(m_command.m_state.primitive(),
                                 elementCount(),
                                 gl::GL_UNSIGNED_INT,
                                 nullptr);
}

void DrawExecution::drawArrays() const
{
    m_command.m_vao.drawArrays(m_command.m_state.primitive(),
                               0u,
                               vertexCount());
}

void DrawExecution::drawArraysInstanced() const
{
    m_command.m_vao.drawArraysInstanced(m_command.m_state.primitive(),
                                        0u,
                                        vertexCount(),
                                        instanceCount());
}

unsigned int DrawExecution::elementCount() const
{
    assert(m_command.m_indexBuffer);
    return m_command.m_indexBuffer->count();
}

unsigned int DrawExecution::vertexCount() const
{
    Assert(!m_command.m_vertexBuffers.empty(), "No vertex buffer bound to command");

    auto ref = m_command.m_vertexBuffers[0].ranged ?
               m_command.m_vertexBuffers[0].count :
               m_command.m_vertexBuffers[0].buffer.get().count();

    {
        for (auto b = 1u; b < m_command.m_vertexBuffers.size(); b++)
        {
            auto cmp = m_command.m_vertexBuffers[b].ranged ?
                       m_command.m_vertexBuffers[b].count :
                       m_command.m_vertexBuffers[b].buffer.get().count();
            assert(cmp == ref);
        }
    }

    return ref;
}

unsigned int DrawExecution::instanceCount() const
{
    assert(!m_command.m_instanceBuffers.empty());

    auto & buffer = m_command.m_instanceBuffers[0].first.get();
    auto divisor = m_command.m_instanceBuffers[0].second;

    auto ref = buffer.count() * divisor;

    for (auto b = 1u; b < m_command.m_instanceBuffers.size(); b++)
    {
        auto & buffer = m_command.m_instanceBuffers[b].first.get();
        auto divisor = m_command.m_instanceBuffers[b].second;

        auto test = buffer.count() * divisor;

        Assert(test == ref, "Differing instance count in buffer " + std::to_string(b) + ": " + std::to_string(test) + " != " + std::to_string(ref));
    }

    return ref;
}

void DrawExecution::applyDepthState()
{
    auto & state = m_command.m_state.depthState();

    Assert(!state.depthMask() || state.depthTest(), "Combination not implemented yet, see glDepthFunc");

    m_glStateManager.enableDepthTest(state.depthTest());
    m_glStateManager.setDepthMask(state.depthMask());
}

void DrawExecution::applyBlendState()
{
    auto & state = m_command.m_state.blendState();

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
    auto & state = m_command.m_state.cullState();

    m_glStateManager.enableCullFace(state.enabled());

    if (!state.enabled())
    {
        return;
    }

    m_glStateManager.setCullFace(state.mode());
}

void DrawExecution::applyRasterizerState()
{
    auto & state = m_command.m_state.rasterizerState();

    m_glStateManager.setPointSize(state.pointSize());
    m_glStateManager.setLineWidth(state.lineWidth());
}

void DrawExecution::applyStencilState()
{
    auto & state = m_command.m_state.stencilState();

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


