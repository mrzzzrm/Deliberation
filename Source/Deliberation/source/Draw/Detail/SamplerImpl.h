#pragma once

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Core/Optional.h>

#include "../GL/GLSamplerState.h"

namespace deliberation
{

class GLStateManager;

namespace detail
{

class TextureImpl;

class SamplerImpl
{
public:
    SamplerImpl(gl::GLenum uniformType,
                gl::GLenum valueType,
                gl::GLuint location);

    gl::GLenum textureType() const;

    GLSamplerState      glSampler;
    gl::GLenum          type;
    gl::GLenum          valueType;
    gl::GLuint          location;
    const TextureImpl * texture;
};

}

}


