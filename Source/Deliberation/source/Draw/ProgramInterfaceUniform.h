#pragma once

#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Core/Types.h>



namespace deliberation
{

class ProgramInterfaceUniform final
{
public:
    ProgramInterfaceUniform(const std::string & name,
                            Type type,
                            gl::GLint location,
                            gl::GLuint arraySize);

    const std::string & name() const;
    Type type() const;
    gl::GLint location() const;
    gl::GLuint arraySize() const;
    bool isArray() const;

    std::string toString() const;

    bool operator==(const ProgramInterfaceUniform & other) const;
    bool operator!=(const ProgramInterfaceUniform & other) const;

private:
    std::string m_name;
    Type        m_type;
    gl::GLint   m_location;
    gl::GLuint  m_arraySize;
};

}

