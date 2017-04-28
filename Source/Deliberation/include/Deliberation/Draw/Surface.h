#pragma once

#include <memory>



#include <Deliberation/Draw/SurfaceDownload.h>
#include <Deliberation/Draw/Texture.h>

namespace deliberation
{

namespace detail
{
    class FramebufferImpl;
    class TextureImpl;
}

class DrawContext;
class SurfaceDownloadImpl;

class Surface final
{
public:
    Surface();
    Surface(const Surface & surface);

    DrawContext & drawContext() const;
    Texture texture() const;
    unsigned int face() const;
    unsigned int width() const;
    unsigned int height() const;
    PixelFormat format() const;

    SurfaceDownload download() const;

    bool operator==(const Surface & other) const;
    bool operator!=(const Surface & other) const;

private:
    friend class Framebuffer;
    friend class SurfaceDownloadImpl;
    friend class detail::FramebufferImpl;
    friend class detail::TextureImpl;

private:
    Surface(const std::shared_ptr<detail::TextureImpl> & texture,
            unsigned int face);

private:
    std::shared_ptr<detail::TextureImpl>    m_texture;
    unsigned int                            m_face;
};

}

