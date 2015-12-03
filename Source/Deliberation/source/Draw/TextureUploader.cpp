#include <Deliberation/Draw/TextureUploader.h>

#include <iostream>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/TextureBinary.h>

#include "Detail/TextureImpl.h"

namespace deliberation
{

TextureUploader::TextureUploader(Context & context, const TextureBinary & textureBinary):
    m_textureBinary(textureBinary)
{

}

Texture TextureUploader::upload()
{
    switch (m_textureBinary.type())
    {
    case gl::GL_TEXTURE_2D:       return uploadSurfaces(gl::GL_TEXTURE_2D);
    case gl::GL_TEXTURE_CUBE_MAP: return uploadSurfaces(gl::GL_TEXTURE_CUBE_MAP);
    default:
        Fail("");
    }
}

Texture TextureUploader::uploadSurfaces(gl::GLenum type)
{
    /*
        TODO
            GLStateManager
    */

    gl::GLuint glName;

    gl::glGenTextures(1, &glName);
    Assert(glName != 0, "glGenTextures() failed");

    gl::glBindTexture(type, glName);

    gl::glTexParameteri(type, gl::GL_TEXTURE_BASE_LEVEL, 0);
    gl::glTexParameteri(type, gl::GL_TEXTURE_MAX_LEVEL, 0);
    gl::glTexParameteri(type, gl::GL_TEXTURE_MIN_FILTER, (gl::GLint)gl::GL_LINEAR);
    gl::glTexParameteri(type, gl::GL_TEXTURE_MAG_FILTER, (gl::GLint)gl::GL_LINEAR);

    for (auto f = 0u; f < m_textureBinary.numFaces(); f++)
    {
        auto & surface = m_textureBinary.surface(f);

        gl::glTexImage2D(faceTarget(type, f),
                         0,
                         (gl::GLint)PixelFormatToGLInternalFormat(surface.format()),
                         surface.width(),
                         surface.height(),
                         0,
                         PixelFormatToGLFormat(surface.format()),
                         gl::GL_FLOAT,
                         surface.pixels().data());
    }

    auto impl = std::make_shared<detail::TextureImpl>(glName,
                                                      m_textureBinary.width(),
                                                      m_textureBinary.height(),
                                                      m_textureBinary.numFaces());

    return Texture(impl);
}

gl::GLenum TextureUploader::faceTarget(gl::GLenum type, unsigned int face) const
{
    switch(type)
    {
    case gl::GL_TEXTURE_2D:
        Assert(face == 0u, "");
        return gl::GL_TEXTURE_2D;
    case gl::GL_TEXTURE_CUBE_MAP:
    {
        Assert(face <= 6u, "");
        gl::GLenum targets[] =
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

