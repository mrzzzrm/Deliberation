#pragma once

#include <memory>

#include <glbinding/gl/enum.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class TextureImpl;

struct SurfaceImpl
{
    SurfaceImpl(
        const std::shared_ptr<TextureImpl> & textureImpl,
        u32 face,
        gl::GLenum glTarget
    ):
        textureImpl(textureImpl),
        face(face),
        glTarget(glTarget)
    {}


    std::shared_ptr<TextureImpl>    textureImpl;
    u32                             face = 0;
    gl::GLenum                      glTarget = gl::GL_NONE;
};

}