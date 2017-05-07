#pragma once

#include <string>

#include "ITextureLoaderImpl.h"
#include "TextureLoaderOrigin.h"

namespace deliberation
{
class TextureLoaderFileImpl : public ITextureLoaderImpl
{
  public:
    TextureLoaderFileImpl(const std::string & path, TextureLoaderOrigin origin);

    virtual TextureBinary load() override;

  private:
    std::string         m_path;
    TextureLoaderOrigin m_origin;
};
}
