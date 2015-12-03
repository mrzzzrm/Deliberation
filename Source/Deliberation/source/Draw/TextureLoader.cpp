#include <Deliberation/Draw/TextureLoader.h>

#include "Detail/TextureLoaderCubeFilesImpl.h"
#include "Detail/TextureLoaderDataImpl.h"
#include "Detail/TextureLoaderFileImpl.h"
#include "Detail/TextureLoaderSDLSurfaceImpl.h"

namespace deliberation
{

TextureLoader::TextureLoader(SDL_Surface * surface):
    m_impl(new TextureLoaderSDLSurfaceImpl(surface))
{

}

TextureLoader::TextureLoader(const std::string & path):
    m_impl(new TextureLoaderFileImpl(path, TextureLoaderFileImpl::Origin::LowerLeft))
{
}

TextureLoader::TextureLoader(void * data, size_t length):
    m_impl(new TextureLoaderDataImpl(data, length))
{
}

TextureLoader::TextureLoader(const std::array<std::string, 6> & paths):
    m_impl(new TextureLoaderCubeFilesImpl(paths))
{

}

TextureLoader::~TextureLoader() = default;

TextureBinary TextureLoader::load()
{
    return m_impl->load();
}

}

