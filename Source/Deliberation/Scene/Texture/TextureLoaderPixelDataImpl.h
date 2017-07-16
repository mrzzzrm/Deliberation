#pragma once

#include <glm/glm.hpp>

#include "ITextureLoaderImpl.h"

namespace deliberation
{
class TextureLoaderPixelDataImpl : public ITextureLoaderImpl
{
public:
    TextureLoaderPixelDataImpl(
        const Blob & blob, u32 width, u32 height, PixelFormat format);

    TextureBinary load() override;

private:
    const Blob & m_blob;
    u32          m_width;
    u32          m_height;
    PixelFormat  m_format;
};
}
