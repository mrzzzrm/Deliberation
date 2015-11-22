#pragma once

#include <iostream>

#include "Core/Assert.h"

#include "gladvance/GLSLValue.h"

template<typename T>
void DrawCommand::set(const std::string & name, T && v)
{
    auto & uniform = m_program.layout().uniform(name);
    auto type = uniform.type();

    Assert(type != gl::GL_SAMPLER_2D, "Can't set sampler '" + name + "' to unit index");

    auto location = uniform.location();

    if (m_uniforms.size() <= location)
    {
        m_uniforms.resize(location + 1);
    }

    m_uniforms[location] = gladv::GLSLValue(v);

    d_unsetUniformLocations.erase(location);
}


