#include <Deliberation/Draw/Texture.h>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include "Detail/TextureImpl.h"

namespace deliberation
{

Texture::Texture() = default;

Texture::~Texture() = default;

unsigned int Texture::width() const
{
    return m_impl->width;
}

unsigned int Texture::height() const
{
    return m_impl->height;
}

unsigned int Texture::numFaces() const
{
    return m_impl->numFaces;
}

gl::GLenum Texture::type() const
{
    return m_impl->type;
}

//Surface & Texture::surface(unsigned int face)
//{
//    Assert(face < numFaces(), "");
//    return m_surfaces[face];
//}
//
//const Surface & Texture::surface(unsigned int face) const
//{
//    Assert(face < numFaces(), "");
//    return m_surfaces[face];
//}

Texture::Texture(const std::shared_ptr<detail::TextureImpl> & impl):
    m_impl(impl)
{
}

}

