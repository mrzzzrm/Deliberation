#include "TextureUploadExecution.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/TextureBinary.h>
#include <Deliberation/Draw/TextureUpload.h>
#include <Deliberation/Draw/Texture.h>

#include "Detail/TextureImpl.h"

namespace deliberation
{

TextureUploadExecution::TextureUploadExecution(GLStateManager & glStateManager,
                                               const TextureUpload & upload):
    m_glStateManager(glStateManager),
    m_upload(upload)
{

}

void TextureUploadExecution::perform()
{
    switch (m_upload.binary().type())
    {
    case gl::GL_TEXTURE_2D:       return uploadSurfaces(gl::GL_TEXTURE_2D);
    case gl::GL_TEXTURE_CUBE_MAP: return uploadSurfaces(gl::GL_TEXTURE_CUBE_MAP);
    default:
        Fail("");
    }
}


void TextureUploadExecution::uploadSurfaces(gl::GLenum type)
{
    /*
        TODO
            GLStateManager
    */

    auto & texture = m_upload.texture();
    auto & binary = m_upload.binary();
    auto & impl = *texture.m_impl;

    if (impl.glName == 0u)
    {
        gl::glGenTextures(1, &impl.glName);
        Assert(impl.glName != 0, "glGenTextures() failed");
    }

    gl::glBindTexture(type, impl.glName);

    Assert(binary.numFaces() == impl.surfaces.size(), "Incompatible Texture/Binary types");

    for (auto f = 0u; f < binary.numFaces(); f++)
    {
        auto & binarySurface = binary.surface(f);

        gl::glTexImage2D(faceTarget(type, f),
                         0,
                         (gl::GLint)PixelFormatToGLInternalFormat(binarySurface.format()),
                         binarySurface.width(),
                         binarySurface.height(),
                         0,
                         PixelFormatToGLFormat(binarySurface.format()),
                         gl::GL_FLOAT,
                         binarySurface.pixels().data());

        impl.width = binarySurface.width();
        impl.height = binarySurface.height();
    }
}

gl::GLenum TextureUploadExecution::faceTarget(gl::GLenum type, unsigned int face) const
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
                This is duplicated in DrawOutput!
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

