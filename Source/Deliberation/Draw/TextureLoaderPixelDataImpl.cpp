#include "TextureLoaderPixelDataImpl.h"

namespace deliberation
{

TextureLoaderPixelDataImpl::TextureLoaderPixelDataImpl(const Blob & blob, u32 width, u32 height, PixelFormat format):
    m_blob(blob),
    m_width(width),
    m_height(height),
    m_format(format)
{}

TextureBinary TextureLoaderPixelDataImpl::load()
{
    return TextureBinary(SurfaceBinary(Blob(m_blob), m_width, m_height, m_format));
}

}
