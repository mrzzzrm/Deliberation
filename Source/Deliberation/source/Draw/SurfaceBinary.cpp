#include <Deliberation/Draw/SurfaceBinary.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

SurfaceBinary SurfaceBinary::empty(unsigned int width,
                                   unsigned int height,
                                   PixelFormat format)
{
    SurfaceBinary binary;

    binary.m_width = width;
    binary.m_height = height;
    binary.m_format = format;

    return binary;
}

SurfaceBinary::SurfaceBinary():
    m_width(0u),
    m_height(0u),
    m_format(PixelFormat_None)
{
}

SurfaceBinary::SurfaceBinary(Blob && data,
                             unsigned int width,
                             unsigned int height,
                             PixelFormat format):
    m_data(std::move(data)),
    m_width(width),
    m_height(height),
    m_format(format)
{
    Assert(width * height * PixelFormatBytesPerPixel(format) == m_data.size(),
           "Requested size: " + std::to_string(width) + "x" + std::to_string(height) + "x" + std::to_string(PixelFormatBytesPerPixel(format)) + "; "
           "Data size: " + std::to_string(m_data.size()));
}

const void * SurfaceBinary::data() const
{
    return m_data.ptr();
}

unsigned int SurfaceBinary::width() const
{
    return m_width;
}

unsigned int SurfaceBinary::height() const
{
    return m_height;
}

PixelFormat SurfaceBinary::format() const
{
    return m_format;
}

uint8_t SurfaceBinary::byte(unsigned int x, unsigned int y, unsigned int channel) const
{
    /*
        TODO doesn't work for most cases, e.g. byte overlapping channels
    */

    auto bpp = PixelFormatBytesPerPixel(m_format);
    auto lineLength = bpp * m_width;
    auto offset = y * lineLength + bpp * x + channel * (bpp / PixelFormatChannelsPerPixel(m_format));

    return ((uint8_t*)m_data.ptr())[offset];
}

uint32_t SurfaceBinary::pixelHex(unsigned int x, unsigned y) const
{
    auto shift = 0;
    uint32_t result = 0;
    for (auto c = 0; c < PixelFormatChannelsPerPixel(m_format); c++)
    {
        result |= byte(x, y, c) << shift;
        shift += 8;
    }

    return result;
}

std::string SurfaceBinary::toString() const
{
    return std::string("SurfaceBinary {") + std::to_string(m_width) + "x" + std::to_string(m_height) + ": " + PixelFormatToString(m_format) + "}";
}

}

