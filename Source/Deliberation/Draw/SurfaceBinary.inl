#include <Deliberation/Core/Assert.h>

#include <string>

namespace deliberation
{

template<typename T>
void SurfaceBinary::setPixel(unsigned int x, unsigned int y, const T & value)
{
    Assert(x < m_width && y < m_height, "Out of bounds: " + std::to_string(x) + "/" + std::to_string(y) + " in " +
                                                            std::to_string(m_width) + "/" + std::to_string(m_height));

    auto bpp = m_format.bytesPerPixel();

    Assert(sizeof(T) == bpp, "");

    m_data.write(y * (bpp * m_width) + (x * bpp), value);
}

template<typename T>
T SurfaceBinary::pixel(unsigned int x, unsigned int y) const
{
    Assert(x < m_width && y < m_height, "Out of bounds: " + std::to_string(x) + "/" + std::to_string(y) + " in " +
                                                            std::to_string(m_width) + "/" + std::to_string(m_height));

    auto bpp = m_format.bytesPerPixel();

    Assert(sizeof(T) == bpp, "");

    T value;
    m_data.read(y * (bpp * m_width) + (x * bpp), value);

    return value;
}

}

