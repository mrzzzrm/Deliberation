#include <Deliberation/Draw/Detail/Framebuffer.h>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLStateManager.h>

#include <Deliberation/Draw/Surface.h>

#include "Draw/GL/GLFramebuffer.h"

#include "Draw/Detail/TextureImpl.h"

namespace deliberation
{

namespace detail
{

Framebuffer::Framebuffer():
    m_isBackbuffer(true),
    m_glFramebufferDirty(false),
    m_resolutionDirty(true),
    m_width(0u),
    m_height(0u)
{

}

unsigned int Framebuffer::width() const
{
    Assert(!m_isBackbuffer, "");
    if (m_resolutionDirty)
    {
        updateResolution();
    }
    return m_width;
}

unsigned int Framebuffer::height() const
{
    Assert(!m_isBackbuffer, "");
    if (m_resolutionDirty)
    {
        updateResolution();
    }
    return m_height;
}

bool Framebuffer::isBackbuffer() const
{
    return m_isBackbuffer;
}

const Surface * Framebuffer::surface(unsigned int index) const
{
    if (index < m_surfaces.size())
    {
        return m_surfaces[index];
    }
    else
    {
        return nullptr;
    }
}

const std::vector<Surface*> & Framebuffer::surfaces() const
{
    return m_surfaces;
}

void Framebuffer::setSurface(unsigned int index, Surface * surface)
{
    if (index >= m_surfaces.size())
    {
        m_surfaces.resize(index + 1, nullptr);
    }

    if (m_surfaces[index] != surface)
    {
        m_glFramebufferDirty = true;
    }

    m_surfaces[index] = surface;

    m_isBackbuffer = false;
}

void Framebuffer::setDepthSurface(Surface * surface)
{
    if (m_depthSurface == surface)
    {
        return;
    }

    m_depthSurface = surface;
    m_glFramebufferDirty = true;
    m_isBackbuffer = false;
}

void Framebuffer::bind(GLStateManager & glStateManager) const
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

void Framebuffer::updateFramebufferDesc() const
{
    std::vector<GLFramebufferDesc::ColorAttachment> colorAttachments(m_surfaces.size());

    Assert(m_surfaces.size() > 0, "");

    for (auto rt = 0u; rt < m_surfaces.size(); rt++)
    {
        if (m_surfaces[rt])
        {
            colorAttachments[rt].glName = m_surfaces[rt]->m_texture->glName;

            if (m_surfaces[rt]->m_texture->numFaces == 1)
            {
                colorAttachments[rt].target = gl::GL_TEXTURE_2D;
            }
            else
            {
                Assert(m_surfaces[rt]->m_texture->numFaces == 6, "");

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

                colorAttachments[rt].target = targets[m_surfaces[rt]->face()];
            }
        }
    }

    m_glFramebufferDesc.reset(GLFramebufferDesc(colorAttachments));
}

void Framebuffer::updateResolution() const
{
    bool foundFirst = false;

    for (auto rt = 0u; rt < m_surfaces.size(); rt++)
    {
        if (m_surfaces[rt])
        {
            if (!foundFirst)
            {
                m_width = m_surfaces[rt]->width();
                m_height = m_surfaces[rt]->height();
                foundFirst = true;
            }

            Assert(m_width == m_surfaces[rt]->width(), "");
            Assert(m_height == m_surfaces[rt]->height(), "");
        }
    }

    Assert(foundFirst, "");

    m_resolutionDirty = false;
}

}

}

