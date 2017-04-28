#pragma once

#include <string>

#include <glbinding/gl/enum.h>



namespace deliberation
{

class BlendState final
{
public:
    static BlendState disabled();

public:
    BlendState();
    BlendState(gl::GLenum equation, gl::GLenum sfactor, gl::GLenum dfactor);

    bool enabled() const;
    gl::GLenum equation() const;
    gl::GLenum sfactor() const;
    gl::GLenum dfactor() const;

    std::string toString() const;

private:
    bool m_enabled;
    gl::GLenum m_equation;
    gl::GLenum m_sfactor;
    gl::GLenum m_dfactor;
};

}


