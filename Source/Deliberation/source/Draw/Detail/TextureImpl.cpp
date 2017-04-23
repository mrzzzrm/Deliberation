#include "TextureImpl.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

namespace
{

gl::GLenum faceTarget(gl::GLenum type, unsigned int face)
{
    switch(type)
    {
    case gl::GL_TEXTURE_2D:
        Assert(face == 0u, "");
        return gl::GL_TEXTURE_2D;
    case gl::GL_TEXTURE_CUBE_MAP:
    {
        Assert(face <= 6u, "");

        static gl::GLenum targets[] =
            {
                gl::GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            };
        /*
            TODO
                This is duplicated in Framebuffer!
        */
        return targets[face];
    }
    default:
        Fail("");
    }
}

}

namespace deliberation
{

namespace detail
{

std::shared_ptr<TextureImpl> TextureImpl::build(DrawContext & drawContext,
                                                unsigned int width,
                                                unsigned int height,
                                                unsigned int numFaces,
                                                PixelFormat format)
{
    auto impl = std::make_shared<TextureImpl>(context, numFaces);

    for (auto f = 0u; f < numFaces; f++)
    {
        impl->surfaces.push_back({impl, f});
    }

    impl->width = width;
    impl->height = height;
    impl->format = format;

    return impl;
}

TextureImpl::TextureImpl(DrawContext & drawContext, unsigned int numFaces):
    context(context),
    glName(0u),
    width(0u),
    height(0u),
    numFaces(numFaces)
{
    baseLevel = 0;
    maxLevel = 0;
    minFilter = gl::GL_LINEAR;
    maxFilter = gl::GL_LINEAR;

    if (numFaces == 1)
    {
        type = gl::GL_TEXTURE_2D;
    }
    else
    {
        Assert(numFaces == 6, "");
        type = gl::GL_TEXTURE_CUBE_MAP;
    }
}

void TextureImpl::allocate() const
{
    Assert(glName == 0, "");

    bind();
    texImage2DAllFaces(nullptr);
}

void TextureImpl::upload(const TextureBinary & binary)
{
    bind();

    Assert(binary.numFaces() == surfaces.size(), "Incompatible Texture/Binary types");
    Assert(binary.format() == format, "Incompatible Texture/Binary types");

    texImage2DAllFaces(&binary);

    width = binary.width();
    height = binary.height();
}

void TextureImpl::bind() const
{
    if (glName == 0u)
    {
        gl::glGenTextures(1, &glName);
        Assert(glName != 0, "glGenTextures() failed");

        gl::glBindTexture(type, glName);

        gl::glTexParameteri(type, gl::GL_TEXTURE_BASE_LEVEL, baseLevel);
        gl::glTexParameteri(type, gl::GL_TEXTURE_MAX_LEVEL, maxLevel);
        gl::glTexParameteri(type, gl::GL_TEXTURE_MIN_FILTER, (gl::GLint)minFilter);
        gl::glTexParameteri(type, gl::GL_TEXTURE_MAG_FILTER, (gl::GLint)maxFilter);
    }

    gl::glBindTexture(type, glName);
}

void TextureImpl::texImage2DAllFaces(const TextureBinary * binary) const
{
    for (auto f = 0u; f < numFaces; f++)
    {
        auto * binarySurface = binary ? &binary->surface(f) : nullptr;

        gl::glTexImage2D(faceTarget(type, f),
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

}

