#include <Deliberation/Draw/DrawOutput.h>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Surface.h>

#include "Detail/TextureImpl.h"

namespace deliberation
{

DrawOutput::DrawOutput():
    m_isBackbuffer(true),
    m_glFramebufferDirty(false),
    m_resolutionDirty(true),
    m_width(0u),
    m_height(0u)
{

}

unsigned int DrawOutput::width() const
{
    if (m_resolutionDirty)
    {
        updateResolution();
    }
    return m_width;
}

unsigned int DrawOutput::height() const
{
    if (m_resolutionDirty)
    {
        updateResolution();
    }
    return m_height;
}

bool DrawOutput::isBackbuffer() const
{
    return m_isBackbuffer;
}

const Surface * DrawOutput::renderTarget(unsigned int index) const
{
    if (index < m_renderTargets.size())
    {
        return m_renderTargets[index];
    }
    else
    {
        return nullptr;
    }
}

const std::vector<Surface*> & DrawOutput::renderTargets() const
{
    return m_renderTargets;
}

void DrawOutput::setRenderTarget(unsigned int index, Surface * surface)
{
    if (index >= m_renderTargets.size())
    {
        m_renderTargets.resize(index + 1, nullptr);
    }

    if (m_renderTargets[index] != surface)
    {
        m_glFramebufferDirty = true;
    }

    m_renderTargets[index] = surface;

    m_isBackbuffer = false;
}

void DrawOutput::updateFramebufferDesc() const
{
    std::vector<GLFramebufferDesc::ColorAttachment> colorAttachments(m_renderTargets.size());

    Assert(m_renderTargets.size() > 0, "");

    for (auto rt = 0u; rt < m_renderTargets.size(); rt++)
    {
        if (m_renderTargets[rt])
        {
            colorAttachments[rt].glName = m_renderTargets[rt]->m_texture->glName;

            if (m_renderTargets[rt]->m_texture->numFaces == 1)
            {
                colorAttachments[rt].target = gl::GL_TEXTURE_2D;
            }
            else
            {
                Assert(m_renderTargets[rt]->m_texture->numFaces == 6, "");

                /*
                    TODO
                        This is duplicated in TextureUploader!
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

                colorAttachments[rt].target = targets[m_renderTargets[rt]->face()];
            }
        }
    }

    m_glFramebufferDesc.reset(GLFramebufferDesc(colorAttachments));
}

void DrawOutput::updateResolution() const
{
    bool foundFirst = false;

    for (auto rt = 0u; rt < m_renderTargets.size(); rt++)
    {
        if (m_renderTargets[rt])
        {
            if (!foundFirst)
            {
                m_width = m_renderTargets[rt]->width();
                m_height = m_renderTargets[rt]->height();
                foundFirst = true;
            }

            Assert(m_width == m_renderTargets[rt]->width(), "");
            Assert(m_height == m_renderTargets[rt]->height(), "");
        }
    }

    Assert(foundFirst, "");

    m_resolutionDirty = false;
}

}

