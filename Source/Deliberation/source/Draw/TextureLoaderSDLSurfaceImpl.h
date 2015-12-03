#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "ITextureLoaderImpl.h"

namespace deliberation
{

class TextureLoaderSDLSurfaceImpl:
    public ITextureLoaderImpl
{
public:
    TextureLoaderSDLSurfaceImpl(SDL_Surface * surface);
    virtual ~TextureLoaderSDLSurfaceImpl();

    virtual TextureBinary load() override;

private:
    SDL_Surface * m_surface;
};

}

