#pragma once

#include <string>
#include <type_traits>

#include <glbinding/gl/enum.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API CullState final
{
public:
    static CullState disabled();

public:
    CullState(gl::GLenum cullFace = gl::GL_BACK, gl::GLenum frontFace = gl::GL_CCW);

    bool enabled() const;
    gl::GLenum cullFace() const;
    gl::GLenum frontFace() const;

    void setEnabled(bool enabled);
    void setCullFace(gl::GLenum cullFace);
    void setFrontFace(gl::GLenum frontFace);

    std::string toString() const;

private:
    bool        m_enabled;
    gl::GLenum  m_cullFace;
    gl::GLenum  m_frontFace;
};

}

