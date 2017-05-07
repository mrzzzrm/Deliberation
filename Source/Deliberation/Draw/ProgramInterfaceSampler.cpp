#include <Deliberation/Draw/ProgramInterfaceSampler.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Core/Types.h>
#include <sstream>

namespace deliberation
{
ProgramInterfaceSampler::ProgramInterfaceSampler(
    const std::string & name, SamplerType type, gl::GLint location, u32 size)
    : m_name(name)
    , m_type(type)
    , m_valueType(Type_None)
    , m_location(location)
    , m_size(size)
{
    switch ((gl::GLenum)type)
    {
    case gl::GL_SAMPLER_1D:
    case gl::GL_SAMPLER_2D:
    case gl::GL_SAMPLER_3D:
    case gl::GL_SAMPLER_CUBE:
    case gl::GL_SAMPLER_1D_SHADOW:
    case gl::GL_SAMPLER_2D_SHADOW:
    case gl::GL_SAMPLER_2D_RECT:
    case gl::GL_SAMPLER_2D_RECT_SHADOW: m_valueType = Type_Float; break;
    case gl::GL_UNSIGNED_INT_SAMPLER_1D:
    case gl::GL_UNSIGNED_INT_SAMPLER_2D:
    case gl::GL_UNSIGNED_INT_SAMPLER_3D:
    case gl::GL_UNSIGNED_INT_SAMPLER_CUBE: m_valueType = Type_U32; break;
    case gl::GL_INT_SAMPLER_1D:
    case gl::GL_INT_SAMPLER_2D:
    case gl::GL_INT_SAMPLER_3D:
    case gl::GL_INT_SAMPLER_CUBE: m_valueType = Type_I32; break;
    default: Fail("Unknown sampler format");
    }
}

const std::string & ProgramInterfaceSampler::name() const { return m_name; }

SamplerType ProgramInterfaceSampler::type() const { return m_type; }

Type ProgramInterfaceSampler::valueType() const { return m_valueType; }

gl::GLint ProgramInterfaceSampler::location() const { return m_location; }

u32 ProgramInterfaceSampler::size() const { return m_size; }

std::string ProgramInterfaceSampler::toString() const
{
    std::stringstream stream;
    stream << (gl::GLenum)m_type << " <" << m_name << "> (Size: " << m_size
           << "; Location: " << m_location << ")";
    return stream.str();
}

bool ProgramInterfaceSampler::
     operator==(const ProgramInterfaceSampler & other) const
{
    return m_name == other.m_name && m_type == other.m_type &&
           m_location == other.m_location && m_size == other.m_size;
}

bool ProgramInterfaceSampler::
     operator!=(const ProgramInterfaceSampler & other) const
{
    return !(*this == other);
}
}
