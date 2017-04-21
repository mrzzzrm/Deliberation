#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/TextureBinary.h>

namespace deliberation
{

class DrawContext;
class Texture;
class TextureBinary;

class DELIBERATION_API TextureUpload final
{
public:
    TextureUpload(DrawContext & context,
                  Texture & texture,
                  const TextureBinary & binary);

    DrawContext & context() const;
    Texture & texture() const;
    const TextureBinary & binary() const;

    void schedule();

private:
    DrawContext & m_context;
    Texture & m_texture;
    TextureBinary m_binary;
};

}

