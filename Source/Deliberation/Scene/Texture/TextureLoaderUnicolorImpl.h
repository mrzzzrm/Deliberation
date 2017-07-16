#pragma once

#include <glm/glm.hpp>

#include "ITextureLoaderImpl.h"

namespace deliberation
{
namespace detail
{
    template<typename T>
    Blob
    CreateTextureLoaderUnicolorBlob(const glm::uvec2 & size, const T & color);
}

template<typename T>
class TextureLoaderUnicolorImpl : public ITextureLoaderImpl
{
public:
    TextureLoaderUnicolorImpl(const glm::uvec2 & size, const T & value);

    TextureBinary load() override;

private:
    Blob        createPixelData() const;
    PixelFormat pixelFormat() const;

private:
    glm::uvec2 m_size;
    T          m_value;
};
}

#include "TextureLoaderUnicolorImpl.inl"