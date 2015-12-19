#pragma once

#include <glbinding/gl/types.h>

#include <vector>
#include <functional>

namespace deliberation
{

class GLFramebufferDesc final
{
public:
    struct ColorAttachment
    {
        gl::GLuint glName;
        gl::GLenum target;
    };

public:
    GLFramebufferDesc(const std::vector<ColorAttachment> & colorAttachments);

    const std::vector<ColorAttachment> & colorAttachments() const;

    std::size_t hash() const;

private:
    std::size_t m_hash;
	std::vector<ColorAttachment> m_colorAttachments;
};

}

