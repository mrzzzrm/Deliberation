#pragma once

#include <glbinding/gl/types.h>

namespace deliberation
{

class Texture;

namespace detail
{

class TextureBindingImpl
{
public:
    TextureBindingImpl(gl::GLenum uniformType, gl::GLuint location);

    gl::GLenum type;
    gl::GLuint location;
    const Texture * texture;
};

}

}


