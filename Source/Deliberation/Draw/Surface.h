#pragma once

#include <memory>

#include <Deliberation/Draw/SurfaceDownload.h>

namespace deliberation
{
class FramebufferImpl;
class DrawContext;
class SurfaceDownloadImpl;
class SurfaceImpl;
class Texture;
class TextureImpl;

class Surface final
{
  public:
    Surface() = default;
    Surface(const std::shared_ptr<SurfaceImpl> & impl);

    bool isInitialized() const { return (bool)m_impl; }

    DrawContext & drawContext() const;

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

  private:
    std::shared_ptr<SurfaceImpl> m_impl;
};
}
