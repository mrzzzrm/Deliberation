#include <Deliberation/Draw/GL/GLStateManager.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLFramebufferDesc.h>

#include "Draw/GL/GLFramebuffer.h"

using namespace gl;

namespace deliberation
{

GLStateManager::GLStateManager():
    m_glTextureCubeMapSeamless(false),
    m_glDepthTest(false),
    m_glDepthMask(false),
    m_glBlend(false),
    m_glBlendEquation(GL_INVALID_ENUM),
    m_glBlendFuncSFactor(GL_INVALID_ENUM),
    m_glBlendFuncDFactor(GL_INVALID_ENUM),
    m_glCullFace(false),
    m_glCullFaceMode(GL_INVALID_ENUM),
    m_glPointSize(1.0f),
    m_glLineWidth(1.0f),
    m_glStencilTest(false),
    m_glClearColorRed(0.0f),
    m_glClearColorGreen(0.0f),
    m_glClearColorBlue(0.0f),
    m_glClearColorAlpha(0.0f),
    m_glClearDepth(1.0f),
    m_glClearStencil(0.0f),
    m_glViewportX(0),
    m_glViewportY(0),
    m_glViewportWidth(640),
    m_glViewportHeight(480)
{
    m_glStencilFunc[0] = GL_ALWAYS;
    m_glStencilFunc[1] = GL_ALWAYS;
    m_glStencilRef[0] = 0;
    m_glStencilRef[1] = 0;
    m_glStencilReadMask[0] = 0;
    m_glStencilReadMask[1] = 0;
    m_glStencilWriteMask[0] = 0;
    m_glStencilWriteMask[1] = 0;
    m_glStencilSFail[0] = GL_KEEP;
    m_glStencilSFail[1] = GL_KEEP;
    m_glStencilDPFail[0] = GL_KEEP;
    m_glStencilDPFail[1] = GL_KEEP;
    m_glStencilDPPass[0] = GL_KEEP;
    m_glStencilDPPass[1] = GL_KEEP;

    glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_BLEND);
    glPointSize(1.0f);
    glLineWidth(1.0f);
    glDisable(GL_STENCIL_TEST);
    glStencilFunc(m_glStencilFunc[0], m_glStencilRef[0], m_glStencilReadMask[0]);
    glStencilMask(m_glStencilWriteMask[0]);
    glStencilOp(m_glStencilSFail[0], m_glStencilDPFail[0], m_glStencilDPPass[0]);
    glViewport(m_glViewportX, m_glViewportY, m_glViewportWidth, m_glViewportHeight);

    for (auto & boundBuffer : m_boundBuffers)
    {
        boundBuffer = 0;
    }

    for (auto & boundFramebuffer : m_boundFramebuffers)
    {
        boundFramebuffer = 0;
    }
}

void GLStateManager::enableTextureCubeMapSeamless(bool enabled)
{
    applyEnableDisableState(GL_TEXTURE_CUBE_MAP_SEAMLESS, m_glTextureCubeMapSeamless, enabled);
}

void GLStateManager::enableDepthTest(bool enabled)
{
    applyEnableDisableState(GL_DEPTH_TEST, m_glDepthTest, enabled);
}

void GLStateManager::setDepthMask(bool enabled)
{
    if (enabled == m_glDepthMask)
    {
        return;
    }

    m_glDepthMask = enabled;

    glDepthMask(m_glDepthMask ? GL_TRUE : GL_FALSE);
}

void GLStateManager::enableBlend(bool enabled)
{
    applyEnableDisableState(GL_BLEND, m_glBlend, enabled);
}

void GLStateManager::setBlendEquation(gl::GLenum equation)
{
    if (equation == m_glBlendEquation)
    {
        return;
    }

    m_glBlendEquation = equation;

    glBlendEquation(m_glBlendEquation);
}

void GLStateManager::setBlendFunc(gl::GLenum sfactor, gl::GLenum dfactor)
{
    if (m_glBlendFuncSFactor == sfactor && m_glBlendFuncDFactor == dfactor)
    {
        return;
    }

    m_glBlendFuncSFactor = sfactor;
    m_glBlendFuncDFactor = dfactor;

    glBlendFunc(m_glBlendFuncSFactor, m_glBlendFuncDFactor);
}

void GLStateManager::enableCullFace(bool enabled)
{
    applyEnableDisableState(GL_CULL_FACE, m_glCullFace, enabled);
}

void GLStateManager::setCullFace(gl::GLenum mode)
{
    if (m_glCullFaceMode == mode)
    {
        return;
    }

    m_glCullFaceMode = mode;
}

void GLStateManager::setPointSize(float size)
{
    if (m_glPointSize == size)
    {
        return;
    }

    m_glPointSize = size;

    glPointSize(m_glPointSize);
}

void GLStateManager::setLineWidth(float width)
{
    if (m_glLineWidth == width)
    {
        return;
    }

    m_glLineWidth = width;

    glLineWidth(m_glLineWidth);
}

void GLStateManager::enableStencilTest(bool enabled)
{
    applyEnableDisableState(GL_STENCIL_TEST, m_glStencilTest, enabled);
}

void GLStateManager::setStencilFunc(gl::GLenum func, int ref, unsigned int mask)
{
    if
    (
        m_glStencilFunc[0] == func && m_glStencilFunc[1] == func &&
        m_glStencilRef[0] == ref && m_glStencilRef[1] == ref &&
        m_glStencilReadMask[0] == mask && m_glStencilReadMask[1] == mask
    )
    {
        return;
    }

    m_glStencilFunc[0] = func;
    m_glStencilFunc[1] = func;
    m_glStencilRef[0] = ref;
    m_glStencilRef[1] = ref;
    m_glStencilReadMask[0] = mask;
    m_glStencilReadMask[1] = mask;

    glStencilFunc(m_glStencilFunc[0], m_glStencilRef[0], m_glStencilReadMask[0]);
}

void GLStateManager::setStencilMask(unsigned int mask)
{
    if (m_glStencilWriteMask[0] == mask && m_glStencilWriteMask[1] == mask)
    {
        return;
    }

    m_glStencilWriteMask[0] = mask;
    m_glStencilWriteMask[1] = mask;

    glStencilMask(m_glStencilWriteMask[0]);
}

void GLStateManager::setStencilOp(gl::GLenum sfail, gl::GLenum dpfail, gl::GLenum dppass)
{
    if
    (
        m_glStencilSFail[0] == sfail && m_glStencilSFail[1] == sfail &&
        m_glStencilDPFail[0] == dpfail && m_glStencilDPFail[1] == dpfail &&
        m_glStencilDPPass[0] == dppass && m_glStencilDPPass[1] == dppass
    )
    {
        return;
    }

    m_glStencilSFail[0] = sfail;
    m_glStencilSFail[1] = sfail;
    m_glStencilDPFail[0] = dpfail;
    m_glStencilDPFail[1] = dpfail;
    m_glStencilDPPass[0] = dppass;
    m_glStencilDPPass[1] = dppass;

    glStencilOp(m_glStencilSFail[0], m_glStencilDPFail[0], m_glStencilDPPass[0]);
}

void GLStateManager::setStencilFuncSeparate(gl::GLenum face, gl::GLenum func, int ref, unsigned int mask)
{
    auto index = face == GL_FRONT ? 0 : 1;

    if
    (
        m_glStencilFunc[index] == func &&
        m_glStencilRef[index] == ref &&
        m_glStencilReadMask[index] == mask
    )
    {
        return;
    }

    m_glStencilFunc[index] = func;
    m_glStencilRef[index] = ref;
    m_glStencilReadMask[index] = mask;

    glStencilFuncSeparate(face, m_glStencilFunc[index], m_glStencilRef[index], m_glStencilReadMask[index]);
}

void GLStateManager::setStencilMaskSeparate(gl::GLenum face, unsigned int mask)
{
    auto index = face == GL_FRONT ? 0 : 1;

    if (m_glStencilWriteMask[index] == mask)
    {
        return;
    }

    m_glStencilWriteMask[index] = mask;

    glStencilMaskSeparate(face, m_glStencilWriteMask[index]);
}

void GLStateManager::setStencilOpSeparate(gl::GLenum face,
                                          gl::GLenum sfail,
                                          gl::GLenum dpfail,
                                          gl::GLenum dppass)
{
    auto index = face == GL_FRONT ? 0 : 1;

    if
    (
        m_glStencilSFail[index] == sfail &&
        m_glStencilDPFail[index] == dpfail &&
        m_glStencilDPPass[index] == dppass
    )
    {
        return;
    }

    m_glStencilSFail[index] = sfail;
    m_glStencilDPFail[index] = dpfail;
    m_glStencilDPPass[index] = dppass;

    glStencilOpSeparate(face, m_glStencilSFail[index], m_glStencilDPFail[index], m_glStencilDPPass[index]);
}

void GLStateManager::applyEnableDisableState(GLenum state, bool & current, bool target)
{
    if (current == target)
    {
        return;
    }

    current = target;

    if (current)
    {
        glEnable(state);
    }
    else
    {
        glDisable(state);
    }
}

void GLStateManager::bindBuffer(GLenum target, GLuint buffer)
{
    auto targetIndex = 0;

    switch (target)
    {
    case GL_ARRAY_BUFFER:               targetIndex = ArrayBufferTarget; break;
    case GL_COPY_READ_BUFFER:           targetIndex = CopyReadBufferTarget; break;
    case GL_COPY_WRITE_BUFFER:          targetIndex = CopyWriteBufferTarget; break;
    case GL_ELEMENT_ARRAY_BUFFER:       targetIndex = ElementArrayBufferTarget; break;
    case GL_PIXEL_PACK_BUFFER:          targetIndex = PixelPackBufferTarget; break;
    case GL_PIXEL_UNPACK_BUFFER:        targetIndex = PixelUnpackBufferTarget; break;
    case GL_TEXTURE_BUFFER:             targetIndex = TextureBufferTarget; break;
    case GL_TRANSFORM_FEEDBACK_BUFFER:  targetIndex = TransformFeedbackBufferTarget; break;
    case GL_UNIFORM_BUFFER:             targetIndex = UniformBufferTarget; break;
    default:
        Fail("");
    }

    if (m_boundBuffers[targetIndex] == buffer)
    {
        return;
    }

    glBindBuffer(target, buffer);

    m_boundBuffers[targetIndex] = buffer;
}

void GLStateManager::deleteBuffer(gl::GLuint buffer)
{
    for (auto & boundBuffer : m_boundBuffers)
    {
        if (boundBuffer == buffer)
        {
            boundBuffer = 0;
        }
    }

    glDeleteBuffers(1, &buffer);
}

void GLStateManager::setViewport(gl::GLint x, gl::GLint y, gl::GLsizei width, gl::GLsizei height)
{
    if (x == m_glViewportX && y == m_glViewportY && width == m_glViewportWidth && height == m_glViewportHeight)
    {
        return;
    }

    glViewport(x, y, width, height);

    m_glViewportX = x;
    m_glViewportY = y;
    m_glViewportWidth = width;
    m_glViewportHeight = height;
}

void GLStateManager::setClearColor(gl::GLclampf red, gl::GLclampf green, gl::GLclampf blue, gl::GLclampf alpha)
{
    if (red != m_glClearColorRed || green != m_glClearColorGreen || blue != m_glClearColorBlue || alpha != m_glClearColorAlpha)
    {
        glClearColor(red, green, blue, alpha);

        m_glClearColorRed = red;
        m_glClearColorGreen = green;
        m_glClearColorBlue = blue;
        m_glClearColorAlpha = alpha;
    }
}

void GLStateManager::setClearDepth(gl::GLclampd depth)
{
    if (m_glClearDepth != depth)
    {
        glClearDepth(depth);

        m_glClearDepth = depth;
    }
}

void GLStateManager::setClearStencil(gl::GLint s)
{
    if (m_glClearStencil != s)
    {
        glClearStencil(s);

        m_glClearStencil = s;
    }
}

void GLStateManager::genFramebuffers(gl::GLsizei n, gl::GLuint * ids)
{
    glGenFramebuffers(n, ids);
}

void GLStateManager::deleteFramebuffers(gl::GLsizei n, gl::GLuint * framebuffers)
{
    /*
        TODO
            Make sure no used m_glFramebuffers is around anymore!
    */

    for (auto f = 0u; f < n; f++)
    {
        for (auto t = 0u; t < FramebufferTargetCount; t++)
        {
            if (m_boundFramebuffers[t] == framebuffers[f])
            {
                m_boundFramebuffers[t] = 0;
            }
        }
    }

    glDeleteFramebuffers(n, framebuffers);
}

void GLStateManager::bindFramebuffer(gl::GLenum target, gl::GLuint framebuffer)
{
    if (target == GL_DRAW_FRAMEBUFFER || target == GL_FRAMEBUFFER)
    {
        if (m_boundFramebuffers[DrawFramebufferTarget] != framebuffer)
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
            m_boundFramebuffers[DrawFramebufferTarget] = framebuffer;
        }
    }

    if (target == GL_READ_FRAMEBUFFER || target == GL_FRAMEBUFFER)
    {
        if (m_boundFramebuffers[ReadFramebufferTarget] != framebuffer)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
            m_boundFramebuffers[ReadFramebufferTarget] = framebuffer;
        }
    }
}

void GLStateManager::framebufferTexture2D(gl::GLenum target, gl::GLenum attachment, gl::GLenum textarget, gl::GLuint texture, gl::GLint level)
{
    glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

std::shared_ptr<GLFramebuffer> GLStateManager::framebuffer(const GLFramebufferDesc & desc)
{
    auto i = m_glFramebuffers.find(desc.hash());
    if (i != m_glFramebuffers.end())
    {
        if (!i->second.lock())
        {
            i = m_glFramebuffers.end();
        }
    }

    std::shared_ptr<GLFramebuffer> result;

    if (i == m_glFramebuffers.end())
    {
        result = std::make_shared<GLFramebuffer>(*this, desc);
        m_glFramebuffers[desc.hash()] = result;
    }
    else
    {
        result = i->second.lock();
    }

    return result;
}

}

