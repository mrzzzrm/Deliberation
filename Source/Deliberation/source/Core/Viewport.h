#pragma once

#include <string>



namespace deliberation
{

class Viewport final
{
public:
    Viewport();
    Viewport(unsigned int x,
             unsigned int y,
             unsigned int width,
             unsigned int height);

    unsigned int x() const;
    unsigned int y() const;
    unsigned int width() const;
    unsigned int height() const;

    void setX(unsigned int x);
    void setY(unsigned int y);
    void setWidth(unsigned int width);
    void setHeight(unsigned int height);

    bool operator==(const Viewport & other) const;
    bool operator!=(const Viewport & other) const;

    std::string toString() const;

private:
    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_width;
    unsigned int m_height;
};

}

