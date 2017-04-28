#include "TextureLoaderFileImpl.h"
#include "TextureLoaderSDLSurfaceImpl.h"

#include <cinttypes>
#include <iostream>
#include <vector>

#include <SDL_image.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/PixelFormat.h>

//#include "ILUtils.h"

namespace deliberation
{

TextureLoaderFileImpl::TextureLoaderFileImpl(const std::string & path, TextureLoaderOrigin origin):
    m_path(path),
    m_origin(origin)
{

}

TextureBinary TextureLoaderFileImpl::load()
{
    auto * rawSurface = IMG_Load(m_path.c_str());
    if (!rawSurface)
    {
        std::cerr << "Failed to load Texture '" << m_path << "'" << std::endl;
        return TextureBinary();
    }
    TextureBinary binary = TextureLoaderSDLSurfaceImpl(rawSurface, m_origin).load();

    SDL_FreeSurface(rawSurface);

    return binary;
}

}

