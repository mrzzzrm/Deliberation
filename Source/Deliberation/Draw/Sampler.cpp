#include <Deliberation/Draw/Sampler.h>

#include <glbinding/Meta.h>

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
    Assert(texture.type() == m_impl->textureType(), "Texture type mismatch");

    Assert(m_impl->valueType == texture.format().glSamplerValueType(), "Sampler (" +
        glbinding::Meta::getString(m_impl->type) +
        ") isn't compatible with texture of format " + texture.format().toString() + "/" +
        glbinding::Meta::getString(texture.format().glSamplerValueType()));

    m_impl->textureImpl = texture.m_impl;
}

void Sampler::setWrap(TextureWrap wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(0, (gl::GLenum)wrap);
    m_impl->glSampler.setWrap(1, (gl::GLenum)wrap);
    m_impl->glSampler.setWrap(2, (gl::GLenum)wrap);
}

void Sampler::setWrapS(TextureWrap wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    m_impl->glSampler.setWrap(0, (gl::GLenum)wrap);
}

void Sampler::setWrapT(TextureWrap wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    m_impl->glSampler.setWrap(1, (gl::GLenum)wrap);
}

void Sampler::setWrapR(TextureWrap wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    m_impl->glSampler.setWrap(2, (gl::GLenum)wrap);
}

void Sampler::setMinFilter(TextureFilter minFilter)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");

    m_impl->glSampler.setMinFilter((gl::GLenum)minFilter);
}

void Sampler::setMagFilter(TextureFilter magFilter)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    Assert(magFilter == TextureFilter::Nearest || magFilter == TextureFilter::Linear,
           "Invalid mag filter");

    m_impl->glSampler.setMagFilter((gl::GLenum)magFilter);
}

Sampler::Sampler(const std::shared_ptr<SamplerImpl> & impl):
    m_impl(impl)
{

}

}

