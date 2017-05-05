#pragma once

#include <vector>

#include <boost/optional.hpp>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Draw/Framebuffer.h>

namespace deliberation
{

class DrawContext;
class FramebufferImpl;

struct ClearImpl
{
    ClearImpl(DrawContext & drawContext,
              const std::shared_ptr<FramebufferImpl> & framebufferImpl);

    DrawContext &                           drawContext;
    Framebuffer                             framebuffer;

    std::vector<gl::GLenum>                 drawBuffers;
    std::vector<boost::optional<glm::vec4>> colors;
    boost::optional<float>                  depth;
    boost::optional<u32>                    stencil;
};

}

