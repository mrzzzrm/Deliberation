#pragma once

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename T> struct DELIBERATION_API GLSLTypeOf
{
    static constexpr gl::GLenum value = gl::GL_NONE;
};

}

#include <Deliberation/Draw/GL/GLSLTypeOf.inl>
