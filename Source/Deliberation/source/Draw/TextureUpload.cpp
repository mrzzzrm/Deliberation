#include <Deliberation/Draw/TextureUpload.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

TextureUpload::TextureUpload(DrawContext & context,
                             Texture & texture,
                             const TextureBinary & binary):
    m_context(context),
    m_texture(texture),
    m_binary(binary)
{
}

DrawContext & TextureUpload::context() const
{
    return m_context;
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
    m_context.scheduleTextureUpload(*this);
}

}

