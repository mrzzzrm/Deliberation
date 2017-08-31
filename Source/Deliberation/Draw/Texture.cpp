#include <Deliberation/Draw/Texture.h>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/TextureBinary.h>

#include "Detail/TextureImpl.h"
#include "DrawContext.h"

namespace deliberation
{
Texture::Texture(const std::shared_ptr<TextureImpl> & impl) : Base(impl) {}

Texture::Texture(const Surface & surface)
{
    AssertM(surface.m_impl.get(), "Surface is hollow");

    m_impl = surface.m_impl->textureImpl;
}

unsigned int Texture::width() const
{
    AssertM(m_impl.get(), "Texture is hollow");
    return m_impl->width;
}

unsigned int Texture::height() const
{
    AssertM(m_impl.get(), "Texture is hollow");
    return m_impl->height;
}

unsigned int Texture::numFaces() const
{
    AssertM(m_impl.get(), "Texture is hollow");
    return m_impl->numFaces;
}

TextureType Texture::type() const
{
    AssertM(m_impl.get(), "Texture is hollow");
    return m_impl->type;
}

PixelFormat Texture::format() const
{
    AssertM(m_impl.get(), "Texture is hollow");
    return m_impl->format;
}

Surface & Texture::surface(unsigned int face)
{
    AssertM(m_impl.get(), "Texture is hollow");
    AssertM(face < numFaces(), "Face out of index");

    return m_impl->surfaces[face];
}

void Texture::upload(const TextureBinary & binary)
{
    AssertM(m_impl.get(), "Texture is hollow");

    AssertM(binary.width() == width(), "Incompatible Texture/Binary resolution");
    AssertM(
        binary.height() == height(), "Incompatible Texture/Binary resolution");
    AssertM(
        binary.numFaces() == m_impl->surfaces.size(),
        "Incompatible Texture/Binary types");
    AssertM(binary.format() == format(), "Incompatible Texture/Binary types");

    auto & glStateManager = m_impl->drawContext.m_glStateManager;

    glStateManager.bindTexture((gl::GLenum)m_impl->type, m_impl->glName);

    m_impl->texImage2DAllFaces(&binary);
}
}
