#pragma once

#include <memory>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Core/Optional.h>

#include "../GL/GLSamplerState.h"

namespace deliberation
{

class DrawImpl;
class GLStateManager;
class TextureImpl;

class SamplerImpl
{
public:
    SamplerImpl(gl::GLenum uniformType,
                gl::GLenum valueType,
                gl::GLuint location);

    gl::GLenum textureType() const;

    GLSamplerState                  glSampler;
    gl::GLenum                      type;
    gl::GLenum                      valueType;
    gl::GLuint                      location;
    std::shared_ptr<TextureImpl>    textureImpl;
};


}


