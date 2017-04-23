#include "FramebufferImpl.h"

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLStateManager.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Surface.h>

#include "Draw/GL/GLFramebuffer.h"

#include "Draw/Detail/TextureImpl.h"

namespace deliberation
{

namespace detail
{

std::shared_ptr<FramebufferImpl> FramebufferImpl::backbuffer(DrawContext & drawContext,
                                                             unsigned int width,
                                                             unsigned int height)
{
    auto result = std::make_shared<FramebufferImpl>(context);
    result->m_isBackbuffer = true;
    result->m_resolutionDirty = false;
    result->m_width = width;
    result->m_height = height;

    return result;
}

std::shared_ptr<FramebufferImpl> FramebufferImpl::custom(DrawContext & drawContext,
                                                         unsigned int width,
                                                         unsigned int height)
{
    auto result = std::make_shared<FramebufferImpl>(context);
    result->m_isBackbuffer = false;
    result->m_resolutionDirty = false;
    result->m_width = width;
    result->m_height = height;

    return result;
}

DrawContext & FramebufferImpl::drawContext() const
{
    return m_drawContext;
}

unsigned int FramebufferImpl::width() const
{
    if (m_resolutionDirty)
    {
        updateResolution();
    }
    return m_width;
}

unsigned int FramebufferImpl::height() const
{
    if (m_resolutionDirty)
    {
        updateResolution();
    }
    return m_height;
}

bool FramebufferImpl::isBackbuffer() const
{
    return m_isBackbuffer;
}

Surface * FramebufferImpl::renderTarget(unsigned int index)
{
    Assert(!m_isBackbuffer, "");

    if (index < m_renderTargets.size())
    {
        return m_renderTargets[index].engaged() ? &m_renderTargets[index].get() : nullptr;
    }
    else
    {
        return nullptr;
    }
}

const Surface * FramebufferImpl::renderTarget(unsigned int index) const
{
    Assert(!m_isBackbuffer, "");

    if (index < m_renderTargets.size())
    {
        return m_renderTargets[index].engaged() ? &m_renderTargets[index].get() : nullptr;
    }
    else
    {
        return nullptr;
    }
}

const std::vector<Optional<Surface>> & FramebufferImpl::renderTargets() const
{
    Assert(!m_isBackbuffer, "");

    return m_renderTargets;
}

Surface * FramebufferImpl::depthTarget()
{
    return m_depthTarget.ptr();
}

const Surface * FramebufferImpl::depthTarget() const
{
    return m_depthTarget.ptr();
}

void FramebufferImpl::setRenderTarget(unsigned int index, Surface * surface)
{
    if (index >= m_renderTargets.size())
    {
        m_renderTargets.resize(index + 1);
    }

    if (m_renderTargets[index] != surface)
    {
        m_glFramebufferDirty = true;
    }

    if (surface)
    {
        m_renderTargets[index].reset(*surface);
    }
    else
    {
        m_renderTargets[index].disengage();
    }

    m_isBackbuffer = false;
    m_resolutionDirty = true;
}

void FramebufferImpl::setDepthTarget(Surface * surface)
{
    if (m_depthTarget == surface)
    {
        return;
    }

    if (surface)
    {
        m_depthTarget.reset(*surface);
    }
    else
    {
        m_depthTarget.disengage();
    }

    m_glFramebufferDirty = true;
    m_isBackbuffer = false;
    m_resolutionDirty = true;
}

void FramebufferImpl::addRenderTarget(PixelFormat format, int index)
{
    auto texture = m_drawContext.createTexture2D(m_width, m_height, format, true);
    if (index < 0)
    {
        index = m_renderTargets.size();
    }
    m_renderTargetTextures[index] = texture;
    setRenderTarget(index, &texture.surface(0));
}

void FramebufferImpl::addDepthTarget(PixelFormat format)
{
    auto texture = m_drawContext.createTexture2D(m_width, m_height, format, true);
    m_depthTargetTexture.reset(texture);
    setDepthTarget(&texture.surface(0));
}

void FramebufferImpl::bind(GLStateManager & glStateManager) const
{
    if (m_isBackbuffer)
    {
        glStateManager.bindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, 0);
    }
    else
    {
        if (m_glFramebufferDirty)
        {
            updateFramebufferDesc();
            m_glFramebuffer = glStateManager.framebuffer(m_glFramebufferDesc.get());
            m_glFramebufferDirty = false;
        }

        m_glFramebuffer->bind();
    }
}

FramebufferImpl::FramebufferImpl(DrawContext & drawContext):
    m_drawContext(context),
    m_isBackbuffer(true),
    m_glFramebufferDirty(false),
    m_resolutionDirty(true),
    m_width(0u),
    m_height(0u)
{

}

void FramebufferImpl::updateFramebufferDesc() const
{
    std::vector<GLFramebufferDesc::ColorAttachment> colorAttachments(m_renderTargets.size());

    Assert(m_renderTargets.size() > 0 || m_depthTarget.engaged(), "");

    for (auto rt = 0u; rt < m_renderTargets.size(); rt++)
    {
        if (m_renderTargets[rt].engaged())
        {
            auto glName = m_renderTargets[rt].get().m_texture->glName;

            if (glName == 0)
            {
                m_renderTargets[rt].get().m_texture->allocate();
                glName = m_renderTargets[rt].get().m_texture->glName;
            }

            colorAttachments[rt].glName = glName;

            Assert(m_renderTargets[rt].get().m_texture.get(), "");

            if (m_renderTargets[rt].get().m_texture->numFaces == 1)
            {
                colorAttachments[rt].target = gl::GL_TEXTURE_2D;
            }
            else
            {
                Assert(m_renderTargets[rt].get().m_texture->numFaces == 6, "");

                /*
                    TODO
                        This is duplicated in TextureUploadExecution!
                */
                static gl::GLenum targets[] =
                {
                    gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                    gl::GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                    gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                    gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                    gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                    gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                };

                colorAttachments[rt].target = targets[m_renderTargets[rt].get().face()];
            }
        }
    }

    GLFramebufferDesc::DepthAttachment depthAttachment;
    gl::GLuint glName;

    if (m_depthTarget.engaged())
    {
        if (m_depthTarget.get().m_texture->glName == 0)
        {
            m_depthTarget.get().m_texture->allocate();
        }
        glName = m_depthTarget.get().m_texture->glName;
    }
    else
    {
        glName = 0;
    }

    depthAttachment.glName = glName;

    m_glFramebufferDesc.reset(GLFramebufferDesc(colorAttachments, depthAttachment));
}

void FramebufferImpl::updateResolution() const
{
    bool foundFirst = false;

    for (auto rt = 0u; rt < m_renderTargets.size(); rt++)
    {
        if (m_renderTargets[rt].engaged())
        {
            if (!foundFirst)
            {
                m_width = m_renderTargets[rt].get().width();
                m_height = m_renderTargets[rt].get().height();
                foundFirst = true;
            }

            Assert(m_width == m_renderTargets[rt].get().width(), "");
            Assert(m_height == m_renderTargets[rt].get().height(), "");
        }
    }

    Assert(foundFirst, "");

    m_resolutionDirty = false;
}

}

}

