#pragma once

#include <vector>
#include <functional>

#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API GLFramebufferDesc final
{
public:
    struct DepthAttachment
    {
        gl::GLuint glName;
    };

    struct ColorAttachment
    {
        gl::GLuint glName;
        gl::GLenum target;
    };

public:
    GLFramebufferDesc(const std::vector<ColorAttachment> & colorAttachments,
                      DepthAttachment depthAttachment);

    const std::vector<ColorAttachment> & colorAttachments() const;
    const DepthAttachment & depthAttachment() const;

    std::size_t hash() const;

private:
    std::size_t m_hash;
	std::vector<ColorAttachment> m_colorAttachments;
    DepthAttachment m_depthAttachment;
};

}

