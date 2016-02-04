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

        /*
            TODO
                This is duplicated in Framebuffer!
        */
        static gl::GLenum targets[] =
        {
            gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            gl::GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            gl::GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            gl::GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        };
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

std::shared_ptr<TextureImpl> TextureImpl::build(Context & context,
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

TextureImpl::TextureImpl(Context & context, unsigned int numFaces):
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
    /*
        TODO
            GLStateManager
    */

    if (glName == 0u)
    {
        gl::glGenTextures(1, &glName);
        Assert(glName != 0, "glGenTextures() failed");
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
                         (gl::GLint)PixelFormatToGLInternalFormat(format),
                         binarySurface ? binarySurface->width() : width,
                         binarySurface ? binarySurface->height() : height,
                         0,
                         PixelFormatToGLFormat(format),
                         PixelFormatToGLType(format),
                         binarySurface ? binarySurface->data() : nullptr);
    }
}

}

}

