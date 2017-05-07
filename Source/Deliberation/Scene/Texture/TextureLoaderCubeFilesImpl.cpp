#include "TextureLoaderCubeFilesImpl.h"

#include "TextureLoaderFileImpl.h"

namespace deliberation
{
TextureLoaderCubeFilesImpl::TextureLoaderCubeFilesImpl(
    const std::array<std::string, 6> & paths)
    : m_paths(paths)
{
}

TextureBinary TextureLoaderCubeFilesImpl::load()
{
    std::array<SurfaceBinary, 6> surfaces;

    for (auto p = 0u; p < m_paths.size(); p++)
    {
        auto & path = m_paths[p];
        auto   texture =
            TextureLoaderFileImpl(path, TextureLoaderOrigin::UpperLeft).load();
        auto surface = SurfaceBinary(texture.surface(0));

        surfaces[p] = std::move(surface);
    }

    return TextureBinary(std::move(surfaces));
}
}
