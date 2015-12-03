#pragma once

#include <string>

#include "ITextureLoaderImpl.h"

namespace deliberation
{

class TextureLoaderFileImpl:
    public ITextureLoaderImpl
{
public:
    enum class Origin
    {
        LowerLeft,
        UpperLeft
    };

public:
    TextureLoaderFileImpl(const std::string & path, Origin origin);

    virtual TextureBinary load() override;

private:
    std::string m_path;
    Origin m_origin;
};

}

