#pragma once

#include <memory>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/SurfaceDownload.h>
#include <Deliberation/Draw/Texture.h>

namespace deliberation
{

namespace detail
{
    class FramebufferImpl;
    class TextureImpl;
}

class Context;
class SurfaceDownloadImpl;

class DELIBERATION_API Surface final
{
public:
    Surface();
    Surface(const Surface & surface);

    Texture texture() const;
    unsigned int face() const;
    unsigned int width() const;
    unsigned int height() const;

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

