#include <Deliberation/Draw/TextureBinding.h>

#include "Detail/TextureBindingImpl.h"

namespace deliberation
{

void TextureBinding::set(const Texture & texture)
{
    m_impl.texture = &texture;
}

TextureBinding::TextureBinding(detail::TextureBindingImpl & impl):
    m_impl(impl)
{

}

}

