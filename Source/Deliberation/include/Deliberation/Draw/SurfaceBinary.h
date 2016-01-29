#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/PixelFormat.h>

namespace deliberation
{

class DELIBERATION_API SurfaceBinary final
{
public:
    static SurfaceBinary empty(unsigned int width,
                               unsigned int height,
                               PixelFormat format);

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

    std::string toString() const;

private:
    std::shared_ptr<std::vector<float>> m_pixels;
    unsigned int                        m_width;
    unsigned int                        m_height;
    PixelFormat                         m_format;
};

}

