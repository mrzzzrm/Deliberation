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

void Sampler::setWrap(gl::GLenum wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(0, wrap);
    m_impl->glSampler.setWrap(1, wrap);
    m_impl->glSampler.setWrap(2, wrap);
}

void Sampler::setWrapS(gl::GLenum wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(0, wrap);
}

void Sampler::setWrapT(gl::GLenum wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(1, wrap);
}

void Sampler::setWrapR(gl::GLenum wrap)
{
    Assert((bool)m_impl, "Hollow Sampler can't be set");
    m_impl->glSampler.setWrap(2, wrap);
}

Sampler::Sampler(const std::shared_ptr<SamplerImpl> & impl):
    m_impl(impl)
{

}

}

