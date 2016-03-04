#pragma once

#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API ProgramInterfaceUniform final
{
public:
    ProgramInterfaceUniform(const std::string & name,
                            gl::GLenum type,
                            gl::GLint location,
                            gl::GLuint arraySize);

    const std::string & name() const;
    gl::GLenum type() const;
    gl::GLint location() const;
    gl::GLuint arraySize() const;
    bool isArray() const;

    std::string toString() const;

    bool operator==(const ProgramInterfaceUniform & other) const;
    bool operator!=(const ProgramInterfaceUniform & other) const;

private:
    std::string m_name;
    gl::GLenum  m_type;
    gl::GLint   m_location;
    gl::GLuint  m_arraySize;
};

}

