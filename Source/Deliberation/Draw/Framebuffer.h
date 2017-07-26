#pragma once

#include <memory>
#include <vector>

#include <boost/optional.hpp>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/AbstractDrawObject.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/Surface.h>

namespace deliberation
{
class ClearImpl;
class ClearExecution;
class DrawContext;
class Surface;
class FramebufferImpl;

struct RenderTarget final
{
    Surface     surface;
    std::string name;
};

struct RenderTargetDesc final
{
    // For DepthTargets
    RenderTargetDesc(const Surface & surface) : surface(surface) {}

    RenderTargetDesc(PixelFormat format) : format(format) {}

    RenderTargetDesc(const Surface & surface, const std::string & name)
        : surface(surface), name(name)
    {
    }

    RenderTargetDesc(PixelFormat format, const std::string & name)
        : format(format), name(name)
    {
    }

    // Surface is valid if format != None. Could use variant<>, yeah...
    PixelFormat format = PixelFormat_None;
    Surface     surface;

    std::string name;
};

struct FramebufferDesc final
{
public:
    FramebufferDesc() = default;

    FramebufferDesc(
        u32                                       width,
        u32                                       height,
        const std::vector<RenderTargetDesc> &     colorTargetDescs,
        const boost::optional<RenderTargetDesc> & depthTargetDesc = {},
        const std::string &                       name = "Unnamed");

    u32                               width = 0;
    u32                               height = 0;
    std::vector<RenderTargetDesc>     colorTargetDescs;
    boost::optional<RenderTargetDesc> depthTargetDesc;
    std::string                       name = "Unnamed";
};

class Framebuffer final:
    public AbstractDrawObject<FramebufferImpl>
{
public:
    Framebuffer() = default;
    Framebuffer(const std::shared_ptr<FramebufferImpl> & impl);

    bool isInitialized() const { return (bool)m_impl; }

    const std::string & name() const;

    unsigned int width() const;
    unsigned int height() const;
    glm::uvec2   size() const { return {width(), height()}; }

    bool isBackbuffer() const;

    const std::vector<RenderTarget> & colorTargets() const;

    boost::optional<Surface> colorTarget(const std::string & name);
    Surface &                colorTargetRef(const std::string & name);

    boost::optional<Surface> depthTarget();
    Surface &                depthTargetRef();

    Clear & clear();

    Clear createClear();

    std::string toString() const;

private:
    friend class Clear;
    friend class Draw;
    friend class DrawContext;
    friend class ClearExecution;
    friend class ClearImpl;
    friend class DrawImpl;
    friend class FramebufferBinding;
};
}
