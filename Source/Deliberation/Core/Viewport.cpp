#include <Deliberation/Core/Viewport.h>

namespace deliberation
{
Viewport::Viewport() : m_x(0), m_y(0), m_width(0), m_height(0) {}

Viewport::Viewport(
    unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    : m_x(x), m_y(y), m_width(width), m_height(height)
{
}

unsigned int Viewport::x() const { return m_x; }

unsigned int Viewport::y() const { return m_y; }

unsigned int Viewport::width() const { return m_width; }

unsigned int Viewport::height() const { return m_height; }

void Viewport::setX(unsigned int x) { m_x = x; }

void Viewport::setY(unsigned int y) { m_y = y; }

void Viewport::setWidth(unsigned int width) { m_width = width; }

void Viewport::setHeight(unsigned int height) { m_height = height; }

bool Viewport::operator==(const Viewport & other) const
{
    return m_x == other.m_x && m_y == other.m_y && m_width == other.m_width &&
           m_height == other.m_height;
}

bool Viewport::operator!=(const Viewport & other) const
{
    return !operator==(other);
}

std::string Viewport::toString() const
{
    return "(" + std::to_string(m_x) + " " + std::to_string(m_y) + " " +
           std::to_string(m_width) + " " + std::to_string(m_height) + ")";
}
}
