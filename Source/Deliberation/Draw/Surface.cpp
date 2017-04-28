#include <Deliberation/Draw/Surface.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Texture.h>

#include "Detail/TextureImpl.h"
#include "Detail/SurfaceDownloadImpl.h"

namespace deliberation
{

Surface::Surface():
    m_texture(nullptr),
    m_face(0u)
{

}

Surface::Surface(const Surface & other):
    m_texture(other.m_texture),
    m_face(other.m_face)
{
}

DrawContext & Surface::drawContext() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return m_texture->drawContext;
}

Texture Surface::texture() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return Texture(m_texture);
}

unsigned int Surface::face() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return m_face;
}

unsigned int Surface::width() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return m_texture->width; // No mipmaps yet
}

unsigned int Surface::height() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return m_texture->height; // No mipmaps yet
}

PixelFormat Surface::format() const
{
    return m_texture->format;
}

SurfaceDownload Surface::download() const
{
    Assert(m_texture.get(), "Surface object is hollow");
    return SurfaceDownload(std::make_shared<SurfaceDownloadImpl>(*this));
}

bool Surface::operator==(const Surface & other) const
{
    return m_texture.get() == other.m_texture.get() && m_face == other.m_face;
}

bool Surface::operator!=(const Surface & other) const
{
    return !operator==(other);
}

Surface::Surface(const std::shared_ptr<detail::TextureImpl> & texture,
                 unsigned int face):
    m_texture(texture),
    m_face(face)
{

}

}

