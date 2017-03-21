#pragma once

#include <experimental/optional>
#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

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
    const glm::uvec4 & scissorRect() const;
    bool scissorRectEnabled() const;

    void setPrimitive(gl::GLenum primitive);
    void setPointSize(gl::GLfloat pointSize);
    void setLineWidth(gl::GLfloat lineWidth);
    void enableScissorRect(u32 x, u32 y, u32 width, u32 height);
    void disableScissorRect();

    std::string toString() const;

private:
    gl::GLenum  m_primitive;
    gl::GLfloat m_pointSize;
    gl::GLfloat m_lineWidth;
    glm::uvec4  m_scissorRect;
    bool        m_scissorRectEnabled = false;
};

}

