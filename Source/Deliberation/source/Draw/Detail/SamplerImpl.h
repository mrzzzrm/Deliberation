#pragma once

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Core/Optional.h>

namespace deliberation
{

namespace detail
{

class TextureImpl;

class SamplerImpl
{
public:
    SamplerImpl(gl::GLuint glName, gl::GLenum uniformType, gl::GLuint location);

    gl::GLuint          glName;
    gl::GLenum          type;
    gl::GLuint          location;
    const TextureImpl * texture;
    gl::GLenum          wrap[3];
};

}

}


