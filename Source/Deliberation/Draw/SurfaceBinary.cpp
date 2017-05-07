#include <Deliberation/Draw/SurfaceBinary.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
SurfaceBinary SurfaceBinary::empty(
    unsigned int width, unsigned int height, PixelFormat format)
{
    SurfaceBinary binary;

    binary.m_width = width;
    binary.m_height = height;
    binary.m_format = format;

    return binary;
}

SurfaceBinary::SurfaceBinary()
    : m_width(0u), m_height(0u), m_format(PixelFormat_None)
{
}

SurfaceBinary::SurfaceBinary(
    unsigned int width, unsigned int height, PixelFormat format)
    : m_width(width), m_height(height), m_format(format)
{
    m_data = Blob(expectedDataSize());
}

SurfaceBinary::SurfaceBinary(
    Blob && data, unsigned int width, unsigned int height, PixelFormat format)
    : m_data(std::move(data))
    , m_width(width)
    , m_height(height)
    , m_format(format)
{
    Assert(
        expectedDataSize() == m_data.size(),
        "Requested size: " + std::to_string(width) + "x" +
            std::to_string(height) + "x" +
            std::to_string(m_format.bytesPerPixel()) +
            "; "
            "Data size: " +
            std::to_string(m_data.size()));
}

const void * SurfaceBinary::data() const { return m_data.ptr(); }

unsigned int SurfaceBinary::width() const { return m_width; }

unsigned int SurfaceBinary::height() const { return m_height; }

PixelFormat SurfaceBinary::format() const { return m_format; }

std::string SurfaceBinary::toString() const
{
    return std::string("SurfaceBinary {") + std::to_string(m_width) + "x" +
           std::to_string(m_height) + ": " + m_format.toString() + "}";
}

std::size_t SurfaceBinary::expectedDataSize() const
{
    return m_width * m_height * m_format.bytesPerPixel();
}
}
