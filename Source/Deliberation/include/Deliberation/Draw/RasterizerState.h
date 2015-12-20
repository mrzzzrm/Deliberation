#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API RasterizerState final
{
public:
    RasterizerState();
    RasterizerState(gl::GLfloat pointSize,
                    gl::GLfloat lineWidth);

    gl::GLfloat pointSize() const;
    gl::GLfloat lineWidth() const;

    void setPointSize(gl::GLfloat pointSize);
    void setLineWidth(gl::GLfloat lineWidth);

    std::string toString() const;

private:
    gl::GLfloat m_pointSize;
    gl::GLfloat m_lineWidth;
};

}

