#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Blob.h>

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
    SurfaceBinary(Blob && data,
                  unsigned int width,
                  unsigned int height,
                  PixelFormat format);

    const void * data() const;
    unsigned int width() const;
    unsigned int height() const;
    PixelFormat format() const;

    uint8_t byte(unsigned int x, unsigned int y, unsigned int channel) const;

    uint32_t pixelHex(unsigned int x, unsigned y) const;

    std::string toString() const;

private:
    Blob            m_data;
    unsigned int    m_width;
    unsigned int    m_height;
    PixelFormat     m_format;
};

}

