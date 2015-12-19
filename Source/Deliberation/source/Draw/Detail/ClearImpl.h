#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Detail/Framebuffer.h>

namespace deliberation
{

class Context;

namespace detail
{

class ClearImpl
{
public:
    ClearImpl(Context & context);

public:
    Context &           context;
    glm::vec4           color;
    float               depth;
    gl::GLint           stencil;
    bool                clearColor;
    bool                clearDepth;
    bool                clearStencil;
    detail::Framebuffer framebuffer;
};

}

}

