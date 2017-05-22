#include "TextureImpl.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/TextureBinary.h>

namespace
{
gl::GLenum faceTarget(gl::GLenum type, unsigned int face)
{
    switch (type)
    {
    case gl::GL_TEXTURE_2D: Assert(face == 0u, ""); return gl::GL_TEXTURE_2D;
    case gl::GL_TEXTURE_CUBE_MAP:
    {
        Assert(face <= 6u, "");

        static gl::GLenum targets[] = {
            gl::GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        };

        return targets[face];
    }
    default: Fail("");
    }
}
}

namespace deliberation
{
TextureImpl::TextureImpl(
    DrawContext & drawContext,
    u32           width,
    u32           height,
    u32           numFaces,
    PixelFormat   format)
    : drawContext(drawContext)
    , width(width)
    , height(height)
    , numFaces(numFaces)
    , format(format)
{
    if (numFaces == 1)
    {
        type = TextureType::Texture2d;
    }
    else
    {
        Assert(numFaces == 6, "");
        type = TextureType::CubeMap;
    }

    // Create GL Texture object
    gl::glGenTextures(1, &glName);
    Assert(glName != 0, "Failed to create GL Texture Object");

    drawContext.m_glStateManager.bindTexture((gl::GLenum)type, glName);
    gl::glTexParameteri((gl::GLenum)type, gl::GL_TEXTURE_MAX_LEVEL, 0);

    // Create faces
    texImage2DAllFaces(nullptr);
}

void TextureImpl::setupSurfaces(
    const std::shared_ptr<TextureImpl> & textureImpl)
{
    for (auto f = 0u; f < numFaces; f++)
    {
        surfaces.emplace_back(std::make_shared<SurfaceImpl>(
            textureImpl, f, faceTarget((gl::GLenum)type, f)));
    }
}

void TextureImpl::texImage2DAllFaces(const TextureBinary * binary) const
{
    for (auto f = 0u; f < numFaces; f++)
    {
        auto * binarySurface = binary ? &binary->surface(f) : nullptr;

        gl::glTexImage2D(
            faceTarget((gl::GLenum)type, f),
            0,
            (gl::GLint)format.glInternalFormat(),
            binarySurface ? binarySurface->width() : width,
            binarySurface ? binarySurface->height() : height,
            0,
            format.glFormat(),
            format.glType(),
            binarySurface ? binarySurface->data() : nullptr);
    }
}
}
