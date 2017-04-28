#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Types.h>



namespace deliberation
{

gl::GLenum ElementalTypeToGLType(const ElementalType & type);
gl::GLenum TypeToGLType(const Type & type);
Type GLTypeToType(gl::GLenum glType);

}