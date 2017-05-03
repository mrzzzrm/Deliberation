#include <Deliberation/Draw/Texture.h>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/TextureBinary.h>

#include "Detail/TextureImpl.h"
#include "DrawContext.h"

namespace deliberation
{

Texture::Texture() = default;

Texture::Texture(const std::shared_ptr<TextureImpl> & impl):
    m_impl(impl)
{
}

Texture::Texture(const Surface & surface)
{
    Assert(surface.m_impl.get(), "Surface is hollow");

    m_impl = surface.m_impl->textureImpl;
}

unsigned int Texture::width() const
{
    Assert(m_impl.get(), "Texture is hollow");
    return m_impl->width;
}

unsigned int Texture::height() const
{
    Assert(m_impl.get(), "Texture is hollow");
    return m_impl->height;
}

unsigned int Texture::numFaces() const
{
    Assert(m_impl.get(), "Texture is hollow");
    return m_impl->numFaces;
}

gl::GLenum Texture::type() const
{
    Assert(m_impl.get(), "Texture is hollow");
    return m_impl->glType;
}

PixelFormat Texture::format() const
{
    Assert(m_impl.get(), "Texture is hollow");
    return m_impl->format;
}

Surface & Texture::surface(unsigned int face)
{
    Assert(m_impl.get(), "Texture is hollow");
    Assert(face < numFaces(), "Face out of index");

    return m_impl->surfaces[face];
}

void Texture::upload(const TextureBinary & binary)
{
    Assert(m_impl.get(), "Texture is hollow");

    Assert(binary.width() == width(), "Incompatible Texture/Binary resolution");
    Assert(binary.height() == height(), "Incompatible Texture/Binary resolution");
    Assert(binary.numFaces() == m_impl->surfaces.size(), "Incompatible Texture/Binary types");
    Assert(binary.format() == format(), "Incompatible Texture/Binary types");

    auto & glStateManager = m_impl->drawContext.m_glStateManager;

    glStateManager.bindTexture(m_impl->glType, m_impl->glName);

    m_impl->texImage2DAllFaces(&binary);
}

}

