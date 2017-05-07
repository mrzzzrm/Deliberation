#include <Deliberation/Scene/Texture/TextureLoader.h>

#include "TextureLoaderCubeFilesImpl.h"
#include "TextureLoaderDataImpl.h"
#include "TextureLoaderFileImpl.h"
#include "TextureLoaderPixelDataImpl.h"
#include "TextureLoaderSDLSurfaceImpl.h"
#include "TextureLoaderUnicolorImpl.h"

namespace deliberation
{
TextureLoader::TextureLoader(SDL_Surface * surface)
    : m_impl(new TextureLoaderSDLSurfaceImpl(
          surface, TextureLoaderOrigin::LowerLeft))
{
}

TextureLoader::TextureLoader(const std::string & path)
    : m_impl(new TextureLoaderFileImpl(path, TextureLoaderOrigin::LowerLeft))
{
}

TextureLoader::TextureLoader(void * data, size_t length)
    : m_impl(new TextureLoaderDataImpl(data, length))
{
}

TextureLoader::TextureLoader(const std::array<std::string, 6> & paths)
    : m_impl(new TextureLoaderCubeFilesImpl(paths))
{
}

template<>
TextureLoader::TextureLoader(const glm::uvec2 size, const glm::vec3 & value)
    : m_impl(
          std::make_unique<TextureLoaderUnicolorImpl<glm::vec3>>(size, value))
{
}

TextureLoader::TextureLoader(
    const Blob & blob, u32 width, u32 height, PixelFormat format)
    : m_impl(std::make_unique<TextureLoaderPixelDataImpl>(
          blob, width, height, format))
{
}

TextureLoader::~TextureLoader() = default;

TextureBinary TextureLoader::load() { return m_impl->load(); }
}
