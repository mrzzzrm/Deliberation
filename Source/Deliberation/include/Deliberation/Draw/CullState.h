#pragma once

#include <string>
#include <type_traits>

#include <glbinding/gl/enum.h>

namespace deliberation
{

class CullState final
{
public:
    static CullState disabled();

public:
    CullState();
    CullState(bool enabled, gl::GLenum mode);

    bool enabled() const;
    gl::GLenum mode() const;

    void setEnabled(bool enabled);
    void setMode(gl::GLenum mode);

    std::string toString() const;

private:
    bool m_enabled;
    gl::GLenum m_mode;
};

}

