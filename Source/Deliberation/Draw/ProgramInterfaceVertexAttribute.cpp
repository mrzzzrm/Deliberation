#include <Deliberation/Draw/ProgramInterfaceVertexAttribute.h>

#include <sstream>

#include <glbinding/gl/enum.h>

namespace deliberation
{
ProgramInterfaceVertexAttribute::ProgramInterfaceVertexAttribute()
    : m_name(), m_glLocation(-1), m_type(Type_None), m_size(0)
{
}

ProgramInterfaceVertexAttribute::ProgramInterfaceVertexAttribute(
    const std::string & name,
    u32                 index,
    gl::GLint           glLocation,
    Type                type,
    u32                 size)
    : m_name(name)
    , m_index(index)
    , m_glLocation(glLocation)
    , m_type(type)
    , m_size(size)
{
}

const std::string & ProgramInterfaceVertexAttribute::name() const
{
    return m_name;
}

u32 ProgramInterfaceVertexAttribute::index() const { return m_index; }

gl::GLint ProgramInterfaceVertexAttribute::glLocation() const
{
    return m_glLocation;
}

Type ProgramInterfaceVertexAttribute::type() const { return m_type; }

u32 ProgramInterfaceVertexAttribute::size() const { return m_size; }

bool ProgramInterfaceVertexAttribute::
     operator==(const ProgramInterfaceVertexAttribute & other) const
{
    return m_name == other.m_name && m_glLocation == other.m_glLocation &&
           m_type == other.m_type && m_size == other.m_size;
}

bool ProgramInterfaceVertexAttribute::
     operator!=(const ProgramInterfaceVertexAttribute & other) const
{
    return !(*this == other);
}

std::string ProgramInterfaceVertexAttribute::toString() const
{
    std::stringstream stream;
    stream << m_glLocation << ": " << m_type.name() << " " << m_name
           << " (size: " << m_size << ")";

    return stream.str();
}
}
