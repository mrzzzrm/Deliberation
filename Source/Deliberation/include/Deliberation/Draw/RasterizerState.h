#pragma once

#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API RasterizerState final
{
public:
    RasterizerState();
    RasterizerState(gl::GLenum primitive);
    RasterizerState(gl::GLenum primitive,
                    gl::GLfloat pointSize,
                    gl::GLfloat lineWidth);

    gl::GLenum primitive() const;
    gl::GLfloat pointSize() const;
    gl::GLfloat lineWidth() const;

    void setPrimitive(gl::GLenum primitive);
    void setPointSize(gl::GLfloat pointSize);
    void setLineWidth(gl::GLfloat lineWidth);

    std::string toString() const;

private:
    gl::GLenum  m_primitive;
    gl::GLfloat m_pointSize;
    gl::GLfloat m_lineWidth;
};

}

