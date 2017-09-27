#pragma once

#include <memory>

#include <Deliberation/Draw/AbstractDrawObject.h>
#include <Deliberation/Draw/SurfaceDownload.h>

namespace deliberation
{
class FramebufferImpl;
class DrawContext;
class SurfaceDownloadImpl;
class SurfaceImpl;
class Texture;
class TextureImpl;

class Surface final:
    public AbstractDrawObject<SurfaceImpl>
{
public:
    Surface() = default;
    Surface(const std::shared_ptr<SurfaceImpl> & impl);

    bool isInitialized() const { return (bool)m_impl; }

    u32 face() const;

    u32 width() const;
    u32 height() const;

    PixelFormat format() const;

    SurfaceDownload download() const;

    std::string toString() const;

private:
    friend class Framebuffer;
    friend class FramebufferImpl;
    friend class SurfaceDownloadImpl;
    friend class FramebufferImpl;
    friend class TextureImpl;
    friend class Texture;
};
}
