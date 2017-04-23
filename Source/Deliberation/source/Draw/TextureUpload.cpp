#include <Deliberation/Draw/TextureUpload.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

TextureUpload::TextureUpload(DrawContext & drawContext,
                             Texture & texture,
                             const TextureBinary & binary):
    m_drawContext(context),
    m_texture(texture),
    m_binary(binary)
{
}

DrawContext & TextureUpload::drawContext() const
{
    return m_drawContext;
}

Texture & TextureUpload::texture() const
{
    return m_texture;
}

const TextureBinary & TextureUpload::binary() const
{
    return m_binary;
}

void TextureUpload::schedule()
{
    m_drawContext.scheduleTextureUpload(*this);
}

}

