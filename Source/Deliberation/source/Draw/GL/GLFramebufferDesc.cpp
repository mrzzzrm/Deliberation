#include <Deliberation/Draw/GL/GLFramebufferDesc.h>

#include <Deliberation/Core/HashUtils.h>

namespace deliberation
{

GLFramebufferDesc::GLFramebufferDesc(const std::vector<ColorAttachment> & colorAttachments):
    m_colorAttachments(colorAttachments)
{
}

const std::vector<GLFramebufferDesc::ColorAttachment> & GLFramebufferDesc::colorAttachments() const
{
    return m_colorAttachments;
}

std::size_t GLFramebufferDesc::hash() const
{
    std::size_t hash = 0;

    for (auto & attachment : m_colorAttachments)
    {
        hash = HashCombine(hash, attachment.glName, (int)attachment.target);
    }

    return hash;
}

}

