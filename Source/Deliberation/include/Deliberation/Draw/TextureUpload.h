#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/TextureBinary.h>

namespace deliberation
{

class Context;
class Texture;
class TextureBinary;

class DELIBERATION_API TextureUpload final
{
public:
    TextureUpload(Context & context,
                  Texture & texture,
                  const TextureBinary & binary);

    Context & context() const;
    Texture & texture() const;
    const TextureBinary & binary() const;

    void schedule();

private:
    Context & m_context;
    Texture & m_texture;
    TextureBinary m_binary;
};

}

