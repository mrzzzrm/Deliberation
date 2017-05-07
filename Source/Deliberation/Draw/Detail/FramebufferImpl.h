#pragma once

#include <memory>
#include <vector>

#include <boost/optional.hpp>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>

namespace deliberation
{
class DrawContext;
class FramebufferDesc;
class GLStateManager;
class Surface;

struct RenderTarget final
{
    Surface     surface;
    std::string name;
};

class FramebufferImpl final
{
  public:
    static std::shared_ptr<FramebufferImpl>
    backbuffer(DrawContext & drawContext, u32 width, u32 height);

    static std::shared_ptr<FramebufferImpl>
    custom(DrawContext & drawContext, const FramebufferDesc & framebufferDesc);

  public:
    FramebufferImpl(DrawContext & drawContext);
    ~FramebufferImpl();

    void bind(const std::vector<gl::GLenum> & drawBuffers);
    size_t
    colorTargetIndex(const std::string & name, bool * found = nullptr) const;

    DrawContext & drawContext;
    bool          isBackbuffer;

    std::vector<RenderTarget>     colorTargets;
    boost::optional<RenderTarget> depthTarget;

    gl::GLuint glName = 0;

    u32 width;
    u32 height;

    boost::optional<Clear> clear;
};
}
