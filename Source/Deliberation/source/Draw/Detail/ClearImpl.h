#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Draw/Framebuffer.h>

namespace deliberation
{

class DrawContext;

namespace detail
{

class FramebufferImpl;

class ClearImpl
{
public:
    ClearImpl(DrawContext & context,
              const std::shared_ptr<FramebufferImpl> & framebuffer);

public:
    DrawContext &               context;
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

