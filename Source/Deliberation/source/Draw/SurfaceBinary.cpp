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

SurfaceBinary::SurfaceBinary(std::vector<float> && pixels,
                             unsigned int width,
                             unsigned int height,
                             PixelFormat format):
    m_pixels(std::make_shared<std::vector<float>>(std::move(pixels))),
    m_width(width),
    m_height(height),
    m_format(format)
{
    Assert(width * height * PixelFormatBytesPerPixel(format) == m_pixels->size() * sizeof(float),
           "Requested size: " + std::to_string(width) + "x" + std::to_string(height) + "x" + std::to_string(PixelFormatBytesPerPixel(format)) + "; "
           "Data size: " + std::to_string(m_pixels->size() * sizeof(float)));
}

const std::vector<float> & SurfaceBinary::pixels() const
{
    return *m_pixels;
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

}

