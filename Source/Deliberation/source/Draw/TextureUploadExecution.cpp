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
    auto & texture = m_upload.texture();
    auto & binary = m_upload.binary();
    auto & impl = *texture.m_impl;

    impl.upload(binary);
}

}

