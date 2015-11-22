#pragma once

#include <string>

#include <glbinding/gl/types.h>

namespace deliberation
{

class ProgramInterfaceVertexAttribute final
{
public:
    ProgramInterfaceVertexAttribute();
    ProgramInterfaceVertexAttribute(const std::string & name,
                                 gl::GLint location,
                                 gl::GLenum type,
                                 gl::GLint size);

    const std::string & name() const;
    gl::GLint location() const;
    gl::GLenum type() const;
    gl::GLint size() const;

    bool operator==(const ProgramInterfaceVertexAttribute & other) const;
    bool operator!=(const ProgramInterfaceVertexAttribute & other) const;

    std::string toString() const;

private:
    std::string m_name;
    gl::GLint m_location;
    gl::GLenum m_type;
    gl::GLint m_size;
};

}


