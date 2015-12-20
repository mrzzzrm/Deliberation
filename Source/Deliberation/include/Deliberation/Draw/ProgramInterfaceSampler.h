#pragma once

#include <string.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API ProgramInterfaceSampler final
{
public:
    ProgramInterfaceSampler(const std::string & name,
                            gl::GLenum type,
                            gl::GLint location,
                            gl::GLuint size);

    const std::string & name() const;
    gl::GLenum type() const;
    gl::GLint location() const;
    gl::GLuint size() const;

    std::string toString() const;

    bool operator==(const ProgramInterfaceSampler & other) const;
    bool operator!=(const ProgramInterfaceSampler & other) const;

private:
    std::string m_name;
    gl::GLenum m_type;
    gl::GLint m_location;
    gl::GLuint m_size;
};

}

