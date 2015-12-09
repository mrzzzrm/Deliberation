#include <Deliberation/Draw/TextureBinding.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Texture.h>

#include "Detail/TextureBindingImpl.h"

namespace deliberation
{

TextureBinding::TextureBinding():
    m_impl(nullptr)
{

}

void TextureBinding::set(const Texture & texture)
{
    Assert(m_impl, "Hollow TextureBinding can't be set");
    Assert(texture.type() == m_impl->type, "Texture type mismatch");

    m_impl->texture = &texture;
}

TextureBinding::TextureBinding(detail::TextureBindingImpl & impl):
    m_impl(&impl)
{

}

}

