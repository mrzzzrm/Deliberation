#pragma once

#include <vector>
#include <functional>

#include <glbinding/gl/types.h>

namespace deliberation
{

class GLFramebufferDesc;
class GLStateManager;

class GLFramebuffer final
{
public:
    GLFramebuffer(GLStateManager & glStateManager,
                  const GLFramebufferDesc & desc);
    ~GLFramebuffer();

    void bind();

private:
    GLStateManager &        m_glStateManager;
    gl::GLuint              m_glName;
    std::vector<gl::GLenum> m_drawBuffers;
};

}

#include "GLFramebuffer.inl"
