#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Draw/Framebuffer.h>

namespace deliberation
{

class Context;

namespace detail
{

class FramebufferImpl;

class ClearImpl
{
public:
    ClearImpl(Context & context,
              const std::shared_ptr<FramebufferImpl> & framebuffer);

public:
    Context &               context;
    glm::vec4               color;
    LinearMap<glm::vec4>    colorOverrides;
    float                   depth;
    gl::GLint               stencil;
    bool                    clearColor;
    bool                    clearDepth;
    bool                    clearStencil;
    Framebuffer             framebuffer;
};

}

}

