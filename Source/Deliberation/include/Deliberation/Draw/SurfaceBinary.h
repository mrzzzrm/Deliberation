#pragma once

#include <vector>

#include <Deliberation/Draw/PixelFormat.h>

namespace deliberation
{

class SurfaceBinary final
{
public:
    SurfaceBinary();
    SurfaceBinary(std::vector<float> && pixels,
                  unsigned int width,
                  unsigned int height,
                  PixelFormat format);

    const std::vector<float> & pixels() const;
    unsigned int width() const;
    unsigned int height() const;
    PixelFormat format() const;

private:
    std::vector<float> m_pixels;
    unsigned int m_width;
    unsigned int m_height;
    PixelFormat m_format;
};

}

