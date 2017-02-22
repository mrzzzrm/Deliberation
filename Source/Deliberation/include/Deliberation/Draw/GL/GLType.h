#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

gl::GLenum DELIBERATION_API ElementalTypeToGLType(const ElementalType & type);
gl::GLenum DELIBERATION_API TypeToGLType(const Type & type);
Type DELIBERATION_API GLTypeToType(gl::GLenum glType);

}