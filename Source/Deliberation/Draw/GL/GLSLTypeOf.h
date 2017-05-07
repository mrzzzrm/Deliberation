#pragma once

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

namespace deliberation
{
template<typename T>
struct GLSLTypeOf
{
    static constexpr gl::GLenum value = gl::GL_NONE;
};
}

#include <Deliberation/Draw/GL/GLSLTypeOf.inl>
