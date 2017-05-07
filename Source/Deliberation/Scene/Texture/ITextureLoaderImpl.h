#pragma once

#include <Deliberation/Draw/TextureBinary.h>

namespace deliberation
{
class ITextureLoaderImpl
{
  public:
    virtual ~ITextureLoaderImpl();

    virtual TextureBinary load() = 0;
};
}
