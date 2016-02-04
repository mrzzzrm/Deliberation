#include <Deliberation/Draw/GL/GLFramebufferDesc.h>

#include <Deliberation/Core/HashUtils.h>

namespace deliberation
{

GLFramebufferDesc::GLFramebufferDesc(const std::vector<ColorAttachment> & colorAttachments,
                                     DepthAttachment depthAttachment):
    m_colorAttachments(colorAttachments),
    m_depthAttachment(depthAttachment)
{
}

const std::vector<GLFramebufferDesc::ColorAttachment> & GLFramebufferDesc::colorAttachments() const
{
    return m_colorAttachments;
}

const GLFramebufferDesc::DepthAttachment & GLFramebufferDesc::depthAttachment() const
{
    return m_depthAttachment;
}

std::size_t GLFramebufferDesc::hash() const
{
    std::size_t hash = 0;

    for (auto & attachment : m_colorAttachments)
    {
        hash = HashCombine(hash, attachment.glName, (int)attachment.target);
    }
    hash = HashCombine(hash, m_depthAttachment.glName);

    return hash;
}

}

