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
        u32 index,
        gl::GLint glLocation,
        Type type,
        gl::GLint size);

    const std::string & name() const;
    u32 index() const;
    gl::GLint glLocation() const;
    Type type() const;
    gl::GLint size() const;

    bool operator==(const ProgramInterfaceVertexAttribute & other) const;
    bool operator!=(const ProgramInterfaceVertexAttribute & other) const;

    std::string toString() const;

private:
    std::string     m_name;
    u32             m_index = 0;
    gl::GLint       m_glLocation = 0;
    Type            m_type;
    gl::GLint       m_size = 0;
};

}


