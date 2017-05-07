#pragma once

#include <cstdint>
#include <cstdlib>

namespace deliberation
{
class Color
{
  public:
    Color();
    Color(float r, float g, float b, float a = 1.0f);
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
    Color(const Color & color);

    uint8_t &       operator[](std::size_t index);
    const uint8_t & operator[](std::size_t index) const;

  private:
    uint8_t m_values[4];
};

static const Color Color_Red = Color(1.0f, 0.0f, 0.0f);
static const Color Color_Green = Color(0.0f, 1.0f, 0.0f);
static const Color Color_Blue = Color(0.0f, 0.0f, 1.0f);
static const Color Color_Yellow = Color(1.0f, 1.0f, 0.0f);
static const Color Color_Orange = Color(1.0f, 0.5f, 0.0f);
static const Color Color_White = Color(1.0f, 1.0f, 1.0f);
static const Color Color_Black = Color(0.0f, 0.0f, 0.0f);
}
