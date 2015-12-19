#include <Deliberation/Draw/GL/GLFramebuffer.h>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLStateManager.h>

namespace deliberation
{

GLFramebuffer::GLFramebuffer(GLStateManager & glStateManager,
                             const GLFramebufferDesc & desc):
    m_glStateManager(glStateManager)
{
    glStateManager.genFramebuffers(1, &m_glName);

    bind();

    Assert(desc.colorAttachments().size() > 0, "");

    auto numBoundColorAttachments = 0u;

    for (auto a = 0u; a < desc.colorAttachments().size(); a++)
    {
        if (desc.colorAttachments()[a].glName != 0)
        {
            glStateManager.framebufferTexture2D(gl::GL_DRAW_FRAMEBUFFER,
                                                gl::GL_COLOR_ATTACHMENT0 + a,
                                                desc.colorAttachments()[a].target,
                                                desc.colorAttachments()[a].glName,
                                                0);
            numBoundColorAttachments++;
        }
    }

    Assert(numBoundColorAttachments > 0, "");

    // Framebuffer should be complete now
    auto status = glCheckFramebufferStatus(gl::GL_DRAW_FRAMEBUFFER);
    Assert(status != gl::GL_FRAMEBUFFER_UNDEFINED, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER, "");
    Assert(status != gl::GL_FRAMEBUFFER_UNSUPPORTED, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS, "");
    Assert(status == gl::GL_FRAMEBUFFER_COMPLETE, "");

    // Draw Buffers
    for (auto a = 0u; a < desc.colorAttachments().size(); a++)
    {
        if (desc.colorAttachments()[a].glName != 0)
        {
            m_drawBuffers.push_back(gl::GL_COLOR_ATTACHMENT0 + a);
        }
    }

}

GLFramebuffer::~GLFramebuffer()
{
    m_glStateManager.deleteFramebuffers(1, &m_glName);
}

void GLFramebuffer::bind()
{
    m_glStateManager.bindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, m_glName);
    gl::glDrawBuffers(m_drawBuffers.size(), m_drawBuffers.data());
}

}

