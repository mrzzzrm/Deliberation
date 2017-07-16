#include <Deliberation/Draw/Sampler.h>

#include <glbinding/Meta.h>
#include <glbinding/gl/gl.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Texture.h>

#include "Detail/DrawImpl.h"
#include "Detail/SamplerImpl.h"

namespace deliberation
{
void Sampler::setTexture(const Texture & texture)
{
    Assert((bool)texture.m_impl, "Texture is hollow");
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    Assert(
        (gl::GLenum)texture.type() == m_impl->textureType(),
        "Texture type mismatch");

    Assert(
        m_impl->valueType == texture.format().glSamplerValueType(),
        "Sampler (" + glbinding::Meta::getString(m_impl->type) +
            ") isn't compatible with texture of format " +
            texture.format().toString() + "/" +
            glbinding::Meta::getString(texture.format().glSamplerValueType()));

    m_impl->textureImpl = texture.m_impl;
}

void Sampler::setWrap(TextureWrap wrap)
{
    setWrapR(wrap);
    setWrapT(wrap);
    setWrapS(wrap);
}

void Sampler::setWrapS(TextureWrap wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    setWrap(0, wrap);
}

void Sampler::setWrapT(TextureWrap wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    setWrap(1, wrap);
}

void Sampler::setWrapR(TextureWrap wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    setWrap(2, wrap);
}

void Sampler::setMinFilter(TextureFilter minFilter)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    if (minFilter == m_impl->minFilter) return;

    gl::glSamplerParameteri(
        m_impl->glName, gl::GL_TEXTURE_MIN_FILTER, (gl::GLint)minFilter);

    m_impl->minFilter = minFilter;
}

void Sampler::setMagFilter(TextureFilter magFilter)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    Assert(
        magFilter == TextureFilter::Nearest ||
            magFilter == TextureFilter::Linear,
        "Invalid mag filter");

    if (magFilter == m_impl->magFilter) return;

    gl::glSamplerParameteri(
        m_impl->glName, gl::GL_TEXTURE_MIN_FILTER, (gl::GLint)magFilter);

    m_impl->magFilter = magFilter;
}

Sampler::Sampler(const std::shared_ptr<SamplerImpl> & impl) : m_impl(impl) {}

void Sampler::setWrap(u32 index, TextureWrap wrap)
{
    if (m_impl->wrap[index] == wrap) return;

    static const gl::GLenum mapping[] = {
        gl::GL_TEXTURE_WRAP_S, gl::GL_TEXTURE_WRAP_T, gl::GL_TEXTURE_WRAP_R};

    gl::glSamplerParameteri(m_impl->glName, mapping[index], (gl::GLint)wrap);
    m_impl->wrap[index] = wrap;
}
}
