#include "TextureLoaderSDLSurfaceImpl.h"

#include <cassert>
#include <iostream>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/PixelFormat.h>

/*
    TODO
        Support non-8bit SDL Formats
        Don't create copy of surface if you don't have to
        Don't store in 32F Texture if you don't have to
        (Probably just copy the SDL source for converting a SDL_Surface into a texture)
*/

namespace deliberation
{

TextureLoaderSDLSurfaceImpl::TextureLoaderSDLSurfaceImpl(SDL_Surface * surface):
    m_surface(nullptr)
{
    auto format = surface->format->format;

    switch (format)
    {
    case SDL_PIXELFORMAT_RGB888:
    case SDL_PIXELFORMAT_RGB24:
        m_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
        break;
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_ARGB8888:
    case SDL_PIXELFORMAT_ABGR8888:
        m_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        break;
    default:
        Fail(std::string("Yet unsupported SDL Format: ") + SDL_GetPixelFormatName(format));
    }
}

TextureLoaderSDLSurfaceImpl::~TextureLoaderSDLSurfaceImpl()
{
    SDL_FreeSurface(m_surface);
}

TextureBinary TextureLoaderSDLSurfaceImpl::load()
{
    std::vector<float> pixels;
    std::size_t numChannels = 0;
    auto format = PixelFormat_None;

    switch (m_surface->format->format)
    {
    case SDL_PIXELFORMAT_RGB888:
        numChannels = 3;
        format = PixelFormat_RGB_32_F;
        break;
    case SDL_PIXELFORMAT_RGBA8888:
        numChannels = 4;
        format = PixelFormat_RGBA_32_F;
        break;
    default:
        Fail("Yet unsupported SDL Format");
    }

    std::size_t numPixels = m_surface->w * m_surface->h * numChannels;

    pixels.reserve(numPixels);

    Uint8 r, g, b, a;

    for (auto y = m_surface->h - 1; y >= 0; y--)
    {
        for (auto x = 0; x < m_surface->w; x++)
        {
            auto offset = y * m_surface->pitch + x * m_surface->format->BytesPerPixel;
            uint8_t * pixel = &((uint8_t*)m_surface->pixels)[offset];
            uint32_t value = *(uint32_t*)pixel;

            switch (m_surface->format->format)
            {
            case SDL_PIXELFORMAT_RGB888:
                SDL_GetRGB(value, m_surface->format, &r, &g, &b);
                pixels.push_back(r/255.0f);
                pixels.push_back(g/255.0f);
                pixels.push_back(b/255.0f);
                break;
            case SDL_PIXELFORMAT_RGBA8888:
            case SDL_PIXELFORMAT_ARGB8888:
                SDL_GetRGBA(value, m_surface->format, &r, &g, &b, &a);

                pixels.push_back(r/255.0f);
                pixels.push_back(g/255.0f);
                pixels.push_back(b/255.0f);
                pixels.push_back(a/255.0f);
                break;
            default:
                Fail(std::string("Yet unsupported SDL Format: ") + SDL_GetPixelFormatName(format));

            }
        }
    }

    return TextureBinary(SurfaceBinary(std::move(pixels), m_surface->w, m_surface->h, format));
}

}

