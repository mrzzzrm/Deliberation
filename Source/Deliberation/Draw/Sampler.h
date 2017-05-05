#pragma once

#include <memory>

#include <glbinding/gl/enum.h>

namespace deliberation
{

class SamplerImpl;
class Texture;

enum class TextureWrap: unsigned int
{
    ClampToEdge         = (unsigned int)gl::GL_CLAMP_TO_EDGE,
    MirroredRepeat      = (unsigned int)gl::GL_MIRRORED_REPEAT,
    Repeat              = (unsigned int)gl::GL_REPEAT,
    ClampToBorder       = (unsigned int)gl::GL_CLAMP_TO_BORDER
};

enum class TextureFilter: unsigned int
{
    Nearest                 = (unsigned int)gl::GL_NEAREST,
    Linear                  = (unsigned int)gl::GL_LINEAR,

    // Only for min filter
    NearestMipmapNearest    = (unsigned int)gl::GL_NEAREST_MIPMAP_NEAREST,
    LinearMipmapNearest     = (unsigned int)gl::GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear     = (unsigned int)gl::GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapLinear      = (unsigned int)gl::GL_LINEAR_MIPMAP_LINEAR
};

class Sampler final
{
public:
    Sampler() = default;

    void setTexture(const Texture & texture);

    void setWrap(TextureWrap wrap);
    void setWrapS(TextureWrap wrap);
    void setWrapT(TextureWrap wrap);
    void setWrapR(TextureWrap wrap);

    void setMinFilter(TextureFilter minFilter);
    void setMagFilter(TextureFilter magFilter);

private:
    friend class Draw;

private:
    Sampler(const std::shared_ptr<SamplerImpl> & impl);

private:
    std::shared_ptr<SamplerImpl> m_impl;
};

}

