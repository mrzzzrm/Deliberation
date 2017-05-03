#include <Deliberation/Draw/Surface.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include "Detail/TextureImpl.h"
#include "Detail/SurfaceImpl.h"
#include "Detail/SurfaceDownloadImpl.h"

namespace deliberation
{

Surface::Surface(
    const std::shared_ptr<SurfaceImpl> & impl
):
    m_impl(impl)
{}

DrawContext & Surface::drawContext() const
{
    Assert(m_impl.get(), "Surface object is hollow");

    return m_impl->textureImpl->drawContext;
}

unsigned int Surface::face() const
{
    Assert(m_impl.get(), "Surface object is hollow");

    return m_impl->face;
}

unsigned int Surface::width() const
{
    Assert(m_impl.get(), "Surface object is hollow");

    return m_impl->textureImpl->width; // No mipmaps yet
}

unsigned int Surface::height() const
{
    Assert(m_impl.get(), "Surface object is hollow");

    return m_impl->textureImpl->height; // No mipmaps yet
}

PixelFormat Surface::format() const
{
    return m_impl->textureImpl->format;
}

SurfaceDownload Surface::download() const
{
    Assert(m_impl.get(), "Surface object is hollow");

    return SurfaceDownload(std::make_shared<SurfaceDownloadImpl>(*this));
}

}

