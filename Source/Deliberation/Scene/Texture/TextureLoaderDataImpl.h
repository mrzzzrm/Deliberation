#pragma once

#include <cstddef>

#include "ITextureLoaderImpl.h"

namespace deliberation
{
class TextureLoaderDataImpl : public ITextureLoaderImpl
{
public:
    TextureLoaderDataImpl(void * data, size_t length);

    virtual TextureBinary load() override;

private:
    void * m_data;
    size_t m_length;
};
}
