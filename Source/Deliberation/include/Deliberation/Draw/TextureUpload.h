#pragma once



#include <Deliberation/Draw/TextureBinary.h>

namespace deliberation
{

class DrawContext;
class Texture;
class TextureBinary;

class TextureUpload final
{
public:
    TextureUpload(DrawContext & drawContext,
                  Texture & texture,
                  const TextureBinary & binary);

    DrawContext & drawContext() const;
    Texture & texture() const;
    const TextureBinary & binary() const;

    void schedule();

private:
    DrawContext & m_drawContext;
    Texture & m_texture;
    TextureBinary m_binary;
};

}

