#include <Deliberation/Core/Color.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
Color::Color()
{
    m_values[0] = 0x7F;
    m_values[1] = 0x7F;
    m_values[2] = 0x7F;
    m_values[3] = 0xFF;
}

Color::Color(float r, float g, float b, float a)
{
    m_values[0] = (uint8_t)(r * 255.0f);
    m_values[1] = (uint8_t)(g * 255.0f);
    m_values[2] = (uint8_t)(b * 255.0f);
    m_values[3] = (uint8_t)(a * 255.0f);
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_values[0] = r;
    m_values[1] = g;
    m_values[2] = b;
    m_values[3] = a;
}

Color::Color(const Color & color)
{
    m_values[0] = color.m_values[0];
    m_values[1] = color.m_values[1];
    m_values[2] = color.m_values[2];
    m_values[3] = color.m_values[3];
}

uint8_t & Color::operator[](std::size_t index)
{
    Assert(index < 4, "");
    return m_values[index];
}

const uint8_t & Color::operator[](std::size_t index) const
{
    Assert(index < 4, "");
    return m_values[index];
}
}
