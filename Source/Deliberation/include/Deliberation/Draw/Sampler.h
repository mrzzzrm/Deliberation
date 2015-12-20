#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{
    class SamplerImpl;
}

class Texture;

class DELIBERATION_API Sampler
{
public:
    Sampler();

    void setTexture(const Texture & texture);

    void setWrap(gl::GLenum wrap);
    void setWrapS(gl::GLenum wrap);
    void setWrapT(gl::GLenum wrap);
    void setWrapR(gl::GLenum wrap);

private:
    friend class Draw;

private:
    Sampler(detail::SamplerImpl & impl);

private:
    detail::SamplerImpl * m_impl;
};

}

