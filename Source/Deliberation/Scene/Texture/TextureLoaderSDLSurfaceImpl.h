#pragma once

#include <string>

#include <SDL.h>

#include "ITextureLoaderImpl.h"
#include "TextureLoaderOrigin.h"

namespace deliberation
{
class TextureLoaderSDLSurfaceImpl : public ITextureLoaderImpl
{
  public:
    TextureLoaderSDLSurfaceImpl(
        SDL_Surface * surface, TextureLoaderOrigin origin);
    virtual ~TextureLoaderSDLSurfaceImpl();

    virtual TextureBinary load() override;

  private:
    SDL_Surface *       m_surface = nullptr;
    TextureLoaderOrigin m_origin = TextureLoaderOrigin::LowerLeft;
};
}
