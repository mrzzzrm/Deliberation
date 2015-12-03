#include "TextureLoaderSDLSurfaceImpl.h"

#include <cassert>
#include <iostream>

#include <Deliberation/Draw/PixelFormat.h>

namespace deliberation
{

TextureLoaderSDLSurfaceImpl::TextureLoaderSDLSurfaceImpl(SDL_Surface * surface):
    m_surface(nullptr)
{
    m_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
}

TextureLoaderSDLSurfaceImpl::~TextureLoaderSDLSurfaceImpl()
{
    SDL_FreeSurface(m_surface);
}

TextureBinary TextureLoaderSDLSurfaceImpl::load()
{
    std::vector<float> pixels;

    for (auto y = m_surface->h - 1; y >= 0; y--)
    {
        for (auto x = 0; x < m_surface->w; x++)
        {
            auto offset = y * m_surface->pitch + x * m_surface->format->BytesPerPixel;
            uint8_t * pixel = &((uint8_t*)m_surface->pixels)[offset];

            pixels.push_back(pixel[0]/255.0f);
            pixels.push_back(pixel[1]/255.0f);
            pixels.push_back(pixel[2]/255.0f);
        }
    }

    return TextureBinary(std::move(pixels), m_surface->w, m_surface->h, PixelFormat_RGB_32F);
}

}

