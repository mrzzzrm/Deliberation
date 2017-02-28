#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API ProgramInterfaceVertexAttribute final
{
public:
    ProgramInterfaceVertexAttribute();
    ProgramInterfaceVertexAttribute(
        const std::string & name,
        gl::GLint location,
        Type type,
        gl::GLint size);

    const std::string & name() const;
    gl::GLint location() const;
    Type type() const;
    gl::GLint size() const;

    bool operator==(const ProgramInterfaceVertexAttribute & other) const;
    bool operator!=(const ProgramInterfaceVertexAttribute & other) const;

    std::string toString() const;

private:
    std::string     m_name;
    gl::GLint       m_location;
    Type            m_type;
    gl::GLint       m_size;
};

}


