#pragma once

#include <array>
#include <string>

#include "ITextureLoaderImpl.h"

namespace deliberation
{

class TextureLoaderCubeFilesImpl:
    public ITextureLoaderImpl
{
public:
    TextureLoaderCubeFilesImpl(const std::array<std::string, 6> & paths);

    virtual TextureBinary load() override;

private:
    std::array<std::string, 6> m_paths;
};

}

