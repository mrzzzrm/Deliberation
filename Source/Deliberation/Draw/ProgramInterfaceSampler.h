#pragma once

#include <string.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
enum class SamplerType : unsigned int
{
    Sampler1d = (unsigned int)gl::GL_SAMPLER_1D,
    Sampler2d = (unsigned int)gl::GL_SAMPLER_2D,
    Sampler3d = (unsigned int)gl::GL_SAMPLER_3D,
    Cube = (unsigned int)gl::GL_SAMPLER_CUBE,
    Sampler1dShadow = (unsigned int)gl::GL_SAMPLER_1D_SHADOW,
    Sampler2dShadow = (unsigned int)gl::GL_SAMPLER_2D_SHADOW,
    Sampler2dRect = (unsigned int)gl::GL_SAMPLER_2D_RECT,
    Sampler2dRectShadow = (unsigned int)gl::GL_SAMPLER_2D_RECT_SHADOW,
    IntSampler1d = (unsigned int)gl::GL_INT_SAMPLER_1D,
    IntSampler2d = (unsigned int)gl::GL_INT_SAMPLER_2D,
    IntSampler3d = (unsigned int)gl::GL_INT_SAMPLER_3D,
    IntSampler2dRect = (unsigned int)gl::GL_INT_SAMPLER_2D_RECT,
    IntCube = (unsigned int)gl::GL_INT_SAMPLER_CUBE,
    UintSampler1d = (unsigned int)gl::GL_UNSIGNED_INT_SAMPLER_1D,
    UintSampler2d = (unsigned int)gl::GL_UNSIGNED_INT_SAMPLER_2D,
    UintSampler3d = (unsigned int)gl::GL_UNSIGNED_INT_SAMPLER_3D,
    UintCube = (unsigned int)gl::GL_UNSIGNED_INT_SAMPLER_CUBE,
    UintSampler2dRect = (unsigned int)gl::GL_UNSIGNED_INT_SAMPLER_2D_RECT,
};

class ProgramInterfaceSampler final
{
  public:
    ProgramInterfaceSampler(
        const std::string & name,
        SamplerType         type,
        gl::GLint           location,
        u32                 size);

    const std::string & name() const;
    SamplerType         type() const;
    Type                valueType() const;
    u32                 size() const;

    std::string toString() const;

    bool operator==(const ProgramInterfaceSampler & other) const;
    bool operator!=(const ProgramInterfaceSampler & other) const;

    // GL specific stuff
    gl::GLint location() const;

  private:
    std::string m_name;
    SamplerType m_type;
    Type        m_valueType;
    gl::GLint   m_location;
    u32         m_size;
};
}
