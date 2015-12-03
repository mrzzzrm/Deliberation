#pragma once

#include <array>
#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include <Deliberation/Draw/TextureBinary.h>

namespace deliberation
{

class ITextureLoaderImpl;

class TextureLoader final
{
public:
    // Single face
    TextureLoader(SDL_Surface * surface);
    TextureLoader(const std::string & path);
    TextureLoader(void * data, size_t length);

    // Cube map
    TextureLoader(const std::array<std::string, 6> & paths);

    ~TextureLoader();

    TextureBinary load();

private:
    std::unique_ptr<ITextureLoaderImpl> m_impl;
};

}

