#pragma once

#include <memory>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/Enum.h>

namespace deliberation
{
class DrawContext;
class DrawImpl;
class GLStateManager;
class TextureImpl;

class SamplerImpl
{
  public:
    SamplerImpl(
        DrawContext & drawContext, gl::GLenum uniformType, gl::GLenum valueType, gl::GLuint location);
    ~SamplerImpl();

    gl::GLenum textureType() const;

    DrawContext &                drawContext;
    gl::GLuint                   glName;
    gl::GLenum                   type;
    gl::GLenum                   valueType;
    gl::GLuint                   location;
    std::shared_ptr<TextureImpl> textureImpl;
    TextureWrap                  wrap[3];
    TextureFilter                minFilter;
    TextureFilter                magFilter;
};
}
