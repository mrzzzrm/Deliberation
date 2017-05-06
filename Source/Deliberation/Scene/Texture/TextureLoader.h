#pragma once

#include <array>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include <SDL.h>

#include <Deliberation/Core/Types.h>
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
    TextureLoader(const Blob & blob, u32 width, u32 height, PixelFormat format);

    template<typename T>
    TextureLoader(const glm::uvec2 size, const T & value);

    // Cube map
    TextureLoader(const std::array<std::string, 6> & paths);

    ~TextureLoader();

    TextureBinary load();

private:
    std::unique_ptr<ITextureLoaderImpl> m_impl;
};

}
