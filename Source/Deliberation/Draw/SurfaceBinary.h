#pragma once

#include <memory>
#include <string>
#include <vector>



#include <Deliberation/Core/Blob.h>

#include <Deliberation/Draw/PixelFormat.h>

namespace deliberation
{

class SurfaceBinary final
{
public:
    static SurfaceBinary empty(unsigned int width,
                               unsigned int height,
                               PixelFormat format);

public:
    SurfaceBinary();
    SurfaceBinary(unsigned int width,
                  unsigned int height,
                  PixelFormat format);
    SurfaceBinary(Blob && data,
                  unsigned int width,
                  unsigned int height,
                  PixelFormat format);

    const void * data() const;
    unsigned int width() const;
    unsigned int height() const;
    PixelFormat format() const;

    template<typename T>
    void setPixel(unsigned int x, unsigned int y, const T & value);

    template<typename T>
    T pixel(unsigned int x, unsigned int y) const;

    std::string toString() const;

private:
    std::size_t expectedDataSize() const;

private:
    Blob            m_data;
    unsigned int    m_width;
    unsigned int    m_height;
    PixelFormat     m_format;
};

}

#include <Deliberation/Draw/SurfaceBinary.inl>

