#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Draw/Texture.h>

namespace deliberation
{

class Context;
class TextureBinary;

class TextureUploader final
{
public:
    /*
        TODO
            Should only be called by Context, make it private?
    */
    TextureUploader(Context & context, const TextureBinary & textureBinary);

    Texture upload();

private:
    Texture uploadSurfaces(gl::GLenum type);
    gl::GLenum faceTarget(gl::GLenum type, unsigned int face) const;

private:
    const TextureBinary & m_textureBinary;
};

}

