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
    SamplerImpl(gl::GLuint glName, gl::GLenum uniformType, gl::GLenum valueType, gl::GLuint location);

    gl::GLenum textureType() const;

    gl::GLuint          glName;
    gl::GLenum          type;
    gl::GLenum          valueType;
    gl::GLuint          location;
    const TextureImpl * texture;
    gl::GLenum          wrap[3];
};

}

}


